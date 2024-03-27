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
mat4 modelViewMatrix = viewMatrix * modelMatrix;

uniform vec3 tangent = vec3(1.0, 1.0, 0.0);

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec2 texCoords;
out vec3 tangentLightDirection;
out vec3 tangentCamDirection;

void main()
{  
    vec3 t = normalize(mat3(normalMatrix) * tangent);      // tangent in view space
    vec3 n = normalize(mat3(normalMatrix) * vertexNormal); // normal in view space
    vec3 b = cross(n, t);                                  // binormal is orthogonal to normal and tangent
    // Create tbn matrix (inverted to transform into tangent space)
    mat3 tbn = transpose(mat3(t, b, n)); // Invert tbn using transpose (tbn is orthogonal matrix)

    vec4 viewVertexPosition = modelViewMatrix * vertexPosition;
    vec3 viewCamDirection = -normalize(viewVertexPosition.xyz);
    vec3 viewLightDirection = normalize(light.viewPosition - viewVertexPosition.xyz);

    texCoords = textureCoords;
    tangentCamDirection = tbn * viewCamDirection;
    tangentLightDirection = tbn * viewLightDirection;

    gl_Position = projectionMatrix * viewVertexPosition;
}
