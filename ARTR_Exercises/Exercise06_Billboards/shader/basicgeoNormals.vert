in vec4 vertexPosition;
in vec4 vertexColor;
in vec3 vertexNormal;

out vec4 vcolor;
out vec3 vnormal;
out vec4 vPosition;

void main()
{
    // Pass through the color to the geometry shader
    vcolor = vertexColor;

    // Pass through the vertex position
    vPosition = vertexPosition;

    //Pass through the vertex normal
    vnormal = vertexNormal;
}
