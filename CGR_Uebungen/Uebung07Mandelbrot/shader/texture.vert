uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

layout (location = 0) in vec4 vertexPosition;

in vec2 textureCoords;
out vec2 texCoords;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;

    texCoords=textureCoords;
}
