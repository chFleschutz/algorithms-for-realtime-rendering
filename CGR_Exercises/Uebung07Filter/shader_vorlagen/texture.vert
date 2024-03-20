in vec4 vertexPosition;
in vec4 vertexNormal;
in vec4 vertexColor;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;
uniform mat3 normalMatrix;

out vec2 texCoords;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;

    texCoords=textureCoords;
}
