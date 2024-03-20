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
    return material.emission.a + material.ambient.a + material.diffuse.a + material.specular.a;
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

vec3 lambert(vec3 viewPosition, vec3 viewDirection, vec3 viewNormal)
{
    vec3 color = vec3(0.0);

    //Nur eine Punktlichtquelle wird berücksichtigt. Im allgemeinen Fall summiert man hier in Schleifen
    //über Richtungslichtquellen, Punktlichtquellen und ggf. Spotlights

    //Dieser Schritt (Berechnung der Richtung, aus der das Licht kommt) kann für Richtungslichtquellen entfallen!
    vec3 lightViewDirection = light.viewPosition - viewPosition;
    color = phongKdKaColorWithAttCalc(lightViewDirection, viewNormal, light.ambient,
                       light.diffuse, light.attenuations);
    return color;
}

void main()
{
    //Die Beleuchtungsrechnung findet in view-Koordinanten statt -> alle Größen umrechnen
    vec3 viewPosition = vec4(modelViewMatrix * vertexPosition).xyz;
    vec3 viewDirection = -normalize(viewPosition);
    vec3 viewNormal = normalize(normalMatrix * vertexNormal);
    vec3 vertexColor = lambert(viewPosition, viewDirection, viewNormal);
    color = vec4(vertexColor + material.emission.rgb, computeMaterialAlpha());
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;;
}
