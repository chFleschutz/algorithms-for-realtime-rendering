in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

out vec3 eyeN;
out vec3 eyeVertex;

void main()
{
    eyeVertex = vec3(modelViewMatrix*vertexPosition); //Entspricht dem Vektor von Kamera zum Vertex im KKS
    eyeN = normalMatrix * vertexNormal;
    gl_Position = projectionMatrix*modelViewMatrix*vertexPosition;
}
