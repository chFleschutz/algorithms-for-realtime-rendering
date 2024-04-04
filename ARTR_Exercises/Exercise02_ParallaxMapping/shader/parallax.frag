
uniform struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
} material;

uniform struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPosition;
    vec2 attenuations; // x = linear, y = quadratic
} light;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

in vec2 texCoords;
in vec3 tangentLightDirection;
in vec3 tangentCamDirection;
in vec3 viewPosition;

out vec4 fragColor;

// Constants
vec4 color = vec4(1.0, 0.5, 0.0, 1.0);
float heightScale = 0.1;

// Parallax mapping functions
// see https://learnopengl.com/Advanced-Lighting/Parallax-Mapping 
vec2 parallaxMapping(vec2 uv, vec3 viewDir);
vec2 steepParallaxMapping(vec2 uv, vec3 viewDir);
vec2 parallaxOcclusionMapping(vec2 uv, vec3 viewDir);

// Phong shading functions
vec3 phongAmbient(vec3 ka, vec3 ia);
vec3 phongDiffuse(vec3 lightDir, vec3 normal, vec3 kd, vec3 id);
vec3 phongSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ks, vec3 is, float shininess);
float lightIntensity(float dist, float constant, float linear, float quadratic);

void main()
{
    vec3 lightDir = normalize(tangentLightDirection);
    vec3 viewDir = normalize(tangentCamDirection);

    // Parallax mapping
//  vec2 uv = parallaxMapping(texCoords, viewDir);
//	vec2 uv = steepParallaxMapping(texCoords, viewDir);
	vec2 uv = parallaxOcclusionMapping(texCoords, viewDir);
    if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0)
        discard;

    // Read textures
    vec3 normal = vec3(texture(normalMap, uv));
    vec3 diffuse = vec3(texture(diffuseMap, uv));
    vec3 ambient = vec3(texture(diffuseMap, uv));
    vec3 specular = vec3(texture(diffuseMap, uv));

    // Phong shading
    float lightDist = length(light.viewPosition - viewPosition);
    float intensity = lightIntensity(lightDist, 1.0, light.attenuations.x, light.attenuations.y);

    vec3 ambientColor = phongAmbient(ambient, light.ambient * intensity);
    vec3 diffuseColor = phongDiffuse(lightDir, normal, diffuse, light.diffuse * intensity);
    vec3 specularColor = phongSpecular(lightDir, normal, viewDir, specular, light.specular * intensity, material.shininess);
    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}

vec2 parallaxMapping(vec2 uv, vec3 viewDir)
{
    float depth = texture(depthMap, uv).r;
    vec2 offset = viewDir.xy / viewDir.z * (depth * heightScale);
    return uv - offset;
}

vec2 steepParallaxMapping(vec2 uv, vec3 viewDir)
{
    const float numLayers = 32.0;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    // Shift for each layer
    vec2 shift = viewDir.xy * heightScale;
    vec2 deltaUV = shift / numLayers;

    vec2 currentUV = uv;
    float currentDepthMapValue = texture(depthMap, currentUV).r;
    while(currentLayerDepth < currentDepthMapValue)
	{
		currentUV -= deltaUV;
		currentDepthMapValue = texture(depthMap, currentUV).r;
		currentLayerDepth += layerDepth;
	}

    return currentUV;
}

vec2 parallaxOcclusionMapping(vec2 uv, vec3 viewDir)
{
    const float numLayers = 32.0;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    // Shift for each layer
    vec2 shift = viewDir.xy * heightScale;
    vec2 deltaUV = shift / numLayers;

    vec2 currentUV = uv;
    float currentDepthMapValue = texture(depthMap, currentUV).r;
    while(currentLayerDepth < currentDepthMapValue)
	{
		currentUV -= deltaUV;
		currentDepthMapValue = texture(depthMap, currentUV).r;
		currentLayerDepth += layerDepth;
	}

    vec2 prevUV = currentUV + deltaUV;

    float nextDepth = currentDepthMapValue - currentLayerDepth;
    float prevDepth = texture(depthMap, prevUV).r - currentLayerDepth + layerDepth;

    float weight = nextDepth / (nextDepth - prevDepth);
    return mix(currentUV, prevUV, weight);
}

vec3 phongAmbient(vec3 ka, vec3 ia)
{
	return ka * ia;
}

vec3 phongDiffuse(vec3 lightDir, vec3 normal, vec3 kd, vec3 id)
{
    return kd * id * max(dot(lightDir, normal), 0.0);
}

vec3 phongSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ks, vec3 is, float shininess)
{
    vec3 reflectionDir = reflect(-lightDir, normal);
    return ks * is * pow(max(dot(reflectionDir, viewDir), 0.0), shininess);
}

float lightIntensity(float dist, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * dist + quadratic * dist * dist);
}