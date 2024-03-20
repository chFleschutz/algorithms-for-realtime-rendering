struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPosition;
    vec2 attenuations; // x = linearAttenuation, y = quadraticAttenuation
};

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

//Uniforms für Datenstrukturen zu Lichtquelle(n) und Materialien
uniform Material material;
uniform PointLight light;

in vec3 viewPosition;
in vec3 viewNormal;

out vec4 fragColor;

float computeLightAttenuation(float lightViewDistance, vec2 lightAttenuations)
{
    return 1.0 / (1.0 + lightAttenuations.x * lightViewDistance + lightAttenuations.y * lightViewDistance * lightViewDistance);
}

float computeMaterialAlpha()
{
    return clamp(material.emission.a + material.ambient.a + material.diffuse.a + material.specular.a, 0.0, 1.0);
}

void main()
{
    vec3 viewDirection = -normalize(viewPosition);
    vec3 normal = normalize(viewNormal); // must be normalized, since it is the result of interpolation
    //Das Licht folgt hier der Kamera
    vec3 lightViewDirection;
    lightViewDirection = light.viewPosition - viewPosition;
    //Um es an seinem Ort im Weltkoordinatensystem zu halten, muss die viewMatrix angewandt werden:
    lightViewDirection = vec3(viewMatrix*vec4(light.viewPosition,1.0)) - viewPosition;

    vec3 color = vec3(0.0);

    float lightViewDistance = length(lightViewDirection);
    float lightAttenuation = computeLightAttenuation(lightViewDistance, light.attenuations);
    lightViewDirection /= lightViewDistance;
    float intensity = dot(lightViewDirection, normal);
    float steppedIntensity = floor(10.0 * intensity) / 10.0;
    vec3 ambient = light.ambient.rgb * material.ambient.rgb;
    vec3 diffuse = light.diffuse.rgb * material.diffuse.rgb;
    vec3 specular = light.specular.rgb * material.specular.rgb;
    color += lightAttenuation * steppedIntensity * (ambient + diffuse + specular);

    fragColor = vec4(color + material.emission.rgb, computeMaterialAlpha());
}
