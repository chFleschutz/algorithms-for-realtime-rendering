in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

out vec2 texCoords;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;

    texCoords=textureCoords;
}
