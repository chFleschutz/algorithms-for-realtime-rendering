
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main()
{
    vPosition = vertexPosition;
    vNormal = vertexNormal;
    vTexCoord = textureCoords;
}
