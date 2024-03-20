in vec4 vertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;
uniform mat3 normalMatrix;

void main()
{

    // Calculate the vertex position
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
}
