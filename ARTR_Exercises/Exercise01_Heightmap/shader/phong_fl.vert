uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec4 vertexPosition;
//Die Normalen werden zur Beleuchtungsrechnung als Vertexattribut immer benötigt
in vec3 vertexNormal;

//Vertexfarbe berücksichtigen
in vec4 vertexColor;

//Position und Normale in Kamerakoordinaten an Rasterisierung->Fragment übergeben
out vec3 viewPosition;
out vec3 viewNormal;

//interpolierte Vertexfarbe übergeben
out vec4 interpolatedVertexColor;


void main()
{
    interpolatedVertexColor = vertexColor;

    //Die Beleuchtungsrechnung findet in view-Koordinanten statt -> alle Größen umrechnen
    viewPosition = vec4(modelViewMatrix * vertexPosition).xyz;
    viewNormal = normalize(normalMatrix * vertexNormal);
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;;
}
