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

uniform vec3 tangent = vec3(1.0f,1.0f,0.0f);

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec2 texCoords;
out vec3 tangentLightDirection;
out vec3 tangentCamDirection;

void main()
{  
    // An dieser Stelle muss anhand der Werte "normalMatrix", "vertexNormal" und "tangent" eine TBN-Matrize berechnet werden.
	
    vec4 viewVertexPosition = modelViewMatrix * vertexPosition;
    vec3 viewCamDirection = -normalize(viewVertexPosition.xyz);
    vec3 viewLightDirection = normalize(light.viewPosition - viewVertexPosition.xyz);

    //Hier müssen unsere tangentCamDirection und tangentLightDirection mit der berechneten TBN-Matrize und der entsprechenden View-Direction verrechnet werden.

    texCoords = textureCoords;
    gl_Position = projectionMatrix * viewVertexPosition;
}