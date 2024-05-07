in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec4 vPosition;
out vec3 vNormal;
out vec2 texCoords;

void main()
{
    vPosition = vertexPosition;
    vNormal = vertexNormal;
    texCoords = textureCoords;
}
