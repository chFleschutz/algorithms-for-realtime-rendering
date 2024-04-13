    in vec4 vertexPosition;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

out vec2 texCoords;

void main()
{
    texCoords=textureCoords;
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
 }
