
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform mat4 MVPLight;
uniform mat4 modelViewLight;
uniform mat3 normalMatrixLight;

uniform bool buildingShadowmap;
uniform vec4 color;

out vec4 outputColor;
out vec4 shadowCoord;
out vec3 lightDir;
out vec3 normal;
out vec2 texCoords;

void main()
{
    if (buildingShadowmap)
    {
        gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
    }
    else
    {
        gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;

        //Normale in LKS berechnen
        normal = normalMatrixLight * vertexNormal;

        // Koordinaten für Shadowmap berechnen
        // xy sind die Koordinaten
        // der Eintrag in der Shadowmap gibt dann den Abstand zur LQ
        // Transformation der VertexPos in das LKS
        shadowCoord = MVPLight * modelMatrix * vertexPosition;

        // Richtung der LQ berechnen
        lightDir = normalize(-vec3(modelViewLight * modelMatrix * vertexPosition));

        //Farbe durchreichen
        outputColor = color;
    }
    texCoords = textureCoords;
}
