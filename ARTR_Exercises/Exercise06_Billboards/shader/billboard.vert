in vec4 vertexPosition;
in vec4 vertexColor;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

void main()
{
    // Calculate the vertex position without Projection Matrix
    //Projection is done by following geometry shader
    gl_Position = modelViewMatrix * vertexPosition;
}
