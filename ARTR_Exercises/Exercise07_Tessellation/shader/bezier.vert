in vec4 vertexPosition;
in vec4 vertexNormal;
in vec4 vertexColor;

out vec4 color;

void main()
{
    color = vertexColor;
    gl_Position = vertexPosition;
}
