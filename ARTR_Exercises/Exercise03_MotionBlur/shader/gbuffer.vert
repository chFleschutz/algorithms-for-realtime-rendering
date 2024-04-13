uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix * modelMatrix;
uniform mat3 normalMatrix;

uniform mat4 UoldMVP;

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

out vec2 texCoords;
out vec3 viewNormal;
out vec3 viewPosition;

out vec4 clipOldPosition; //nur xy und w
out vec4 clipNewPosition; //nur xy und w

void main()
{
    viewPosition = vec3(modelViewMatrix * vertexPosition);
    viewNormal = normalize(normalMatrix * vertexNormal);

    //hier müssen die Clip-Space-Positionen vom aktuellen und vorherigen Frame berechnet werden
    clipOldPosition = UoldMVP * vertexPosition;
    clipNewPosition = projectionMatrix * modelViewMatrix * vertexPosition;

    texCoords = textureCoords;
    gl_Position = clipNewPosition;
}
