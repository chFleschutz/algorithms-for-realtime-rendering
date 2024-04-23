uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;
uniform mat3 normalMatrix;

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec2 texCoords;
out vec3 viewNormal;
out vec3 viewPosition;

void main()
{
    viewPosition = vec3(modelViewMatrix * vertexPosition);
    viewNormal = normalize(normalMatrix * vertexNormal);
    texCoords = textureCoords;
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
}
