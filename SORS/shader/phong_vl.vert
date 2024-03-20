//Zunächst Definition der benötigten Datentypen
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

//Uniforms für Datenstrukturen zu Lichtquelle(n) und Materialien
uniform Material material;
uniform PointLight light;

uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec4 vertexPosition;
//Die Normalen werden zur Beleuchtungsrechnung als Vertexattribut immer benötigt
in vec3 vertexNormal;

out vec4 color;

//Im Folgenden Funktionen, die das Beleuchtungsmodell realisieren
//Es sind jeweils einzelne Funktionen, um eine einfachere Wiederverwendbarkeit zu schaffen
float phongKd(vec3 lightViewDirection, vec3 viewNormal)
{
    return max(dot(lightViewDirection, viewNormal), 0.0);
}

float phongKs(vec3 viewDirection, vec3 reflectDirection)
{
    return pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
}

float phongKa()
{
    return(1.0);
}

float computeLightAttenuation(float lightViewDistance, vec2 lightAttenuations)
{
    return 1.0 / (1.0 + lightAttenuations.x * lightViewDistance + lightAttenuations.y * lightViewDistance * lightViewDistance);
}

float computeMaterialAlpha()
{
    return clamp(material.emission.a + material.ambient.a + material.diffuse.a + material.specular.a, 0.0, 1.0);
}

vec3 phongKdKaColor(vec3 lightViewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, float lightAttenuation)
{
    vec3 diffuseColor = phongKd(lightViewDirection, viewNormal) * (lightDiffuse * material.diffuse.rgb);
    vec3 ambientColor = phongKa() * lightAmbient * material.ambient.rgb;
    vec3 color = (diffuseColor + ambientColor) * lightAttenuation;

    return color;
}

vec3 phongKdKaColorWithAttCalc(vec3 unnormalizedLightViewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, vec2 lightAttenuations)
{
    float lightViewDistance = length(unnormalizedLightViewDirection);
    float lightAttenuation = computeLightAttenuation(lightViewDistance, lightAttenuations);
    vec3 lightViewDirection = unnormalizedLightViewDirection / lightViewDistance;
    return phongKdKaColor(lightViewDirection, viewNormal, lightAmbient, lightDiffuse, lightAttenuation);
}

vec3 phongKsColor(vec3 lightViewDirection, vec3 viewDirection, vec3 viewNormal, vec3 lightSpecular, float lightAttenuation)
{
    //Vorsicht, GLSL reflect Funktion verlangt einfallenden Lichtvektor
    return (phongKs(viewDirection, reflect(-lightViewDirection, viewNormal)) * lightAttenuation * (lightSpecular * material.specular.rgb));
}

vec3 phong(vec3 unnormalizedLightViewDirection, vec3 viewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, vec2 lightAttenuations)
{
    float lightViewDistance = length(unnormalizedLightViewDirection);
    float lightAttenuation = computeLightAttenuation(lightViewDistance, lightAttenuations);
    vec3 lightViewDirection = normalize(unnormalizedLightViewDirection);
    vec3 specularColor = phongKsColor(lightViewDirection, viewDirection, viewNormal, lightSpecular, lightAttenuation);
    return phongKdKaColor(lightViewDirection, viewNormal, lightAmbient, lightDiffuse, lightAttenuation) + specularColor;
}

void main()
{
    //Die Beleuchtungsrechnung findet in view-Koordinanten statt -> alle Größen umrechnen
    vec3 viewPosition = vec4(modelViewMatrix * vertexPosition).xyz;
    vec3 viewDirection = -normalize(viewPosition);
    vec3 viewNormal = normalize(normalMatrix * vertexNormal);
    //Das Licht folgt hier der Kamera
    vec3 lightViewDirection;
    lightViewDirection = light.viewPosition - viewPosition;
    //Um es an seinem Ort im Weltkoordinatensystem zu halten, muss die viewMatrix angewandt werden:
    lightViewDirection = vec3(viewMatrix*vec4(light.viewPosition,1.0)) - viewPosition;

    vec3 vertexColor = phong(lightViewDirection, viewDirection, viewNormal, light.ambient, light.diffuse, light.specular, light.attenuations);
    color = vec4(vertexColor + material.emission.rgb, computeMaterialAlpha());
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;;
}
