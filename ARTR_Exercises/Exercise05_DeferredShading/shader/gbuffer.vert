uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec2 texCoords;
out vec3 worldNormal;
out vec3 worldPos;

void main()
{
    // WorldPos
    worldPos = (modelMatrix * vertexPosition).xyz;
    // Worldspace Normals würde man eigentlich am besten auf der CPU berechnen
    mat3 normalMat = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalMat * vertexNormal;
    texCoords = textureCoords;
    gl_Position = projectionMatrix*viewMatrix*vec4(worldPos,1.0);
}
