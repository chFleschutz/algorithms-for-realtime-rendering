
uniform bool hastextureMap0 = false;
uniform bool hastextureMap1 = false;
uniform bool hastextureMap2 = false;
uniform sampler2D textureMap0; //gPosition;
uniform sampler2D textureMap1; //gNormals;
uniform sampler2D textureMap2; //gAlbedo;

uniform vec3 viewPos;
uniform int actualLightsNumber;

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPosition; // Is actualy world position
    vec2 attenuations; 
};

const int MAX_NR_LIGHTS = 100;
uniform PointLight lights[MAX_NR_LIGHTS];

in vec2 texCoords;

out vec4 fragColor;

const vec3 ambientLight = vec3(0.1);
const float shininess = 32.0;
const float maxLightInfluence = 10.0;

// Phong shading functions
vec3 phongAmbient(vec3 ka, vec3 ia);
vec3 phongDiffuse(vec3 lightDir, vec3 normal, vec3 kd, vec3 id);
vec3 phongSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ks, vec3 is, float shininess);
float lightIntensity(float dist, float constant, float linear, float quadratic);

void main()
{            
    // retrieve data from G-buffer
    vec3 fragPos = texture(textureMap0, texCoords).rgb;
    vec3 normal = texture(textureMap1, texCoords).rgb;
    vec3 albedo = texture(textureMap2, texCoords).rgb;

    vec3 viewDir = normalize(viewPos - fragPos);

    // Phong lighting calc
    vec3 ambientColor = phongAmbient(albedo, ambientLight);
    vec3 diffuseColor = vec3(0.0);
    vec3 specularColor = vec3(0.0);
    for (int i = 0; i < actualLightsNumber; i++)
    {
        PointLight light = lights[i];
        float lightDistance = length(light.viewPosition - fragPos);
        // Disable light if it's too far away
        if (lightDistance > maxLightInfluence)
			continue;

        vec3 lightDir = normalize(light.viewPosition - fragPos);
        float intensity = lightIntensity(lightDistance, 1.0, light.attenuations.x, light.attenuations.y);

        diffuseColor += phongDiffuse(lightDir, normal, albedo, light.diffuse) * intensity;
        specularColor += phongSpecular(lightDir, normal, viewDir, albedo, light.specular, shininess) * intensity;
    }
    
    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}  

vec3 phongAmbient(vec3 ka, vec3 ia)
{
	return ka * ia;
}

vec3 phongDiffuse(vec3 lightDir, vec3 normal, vec3 kd, vec3 id)
{
    return kd * id * clamp(dot(lightDir, normal), 0.0, 1.0);
}

vec3 phongSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 ks, vec3 is, float shininess)
{
    float specular = 0.0;

    if (dot(lightDir, normal) > 0.0)
	{
        // Use Blinns half vector
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specular = pow(dot(normal, halfwayDir), shininess);
	}

    return ks * is * specular;
}

float lightIntensity(float dist, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + (linear * dist) + (quadratic * dist * dist));
}