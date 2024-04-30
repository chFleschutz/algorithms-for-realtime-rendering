layout (triangles) in;
layout (line_strip, max_vertices=6) out;

uniform vec4 startColor;
uniform vec4 endColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

in vec4 vcolor[];
in vec3 vnormal[];
in vec4 vPosition[];
out vec4 gcolor;

void main(void)
{
    // Aufgabe 1: 
    // In der Szene liegt eine Sphere. Ueber die erste Zeile "layout (triangles) in" erhalten wir die Dreiecksvertices der Sphere
    // TODO: Größe des VertexArrays mit gl_in.length() durch iterieren
    for(xxx) {
        gcolor = startColor;
        // TODO: gl_Position des Startvertex (Vertex auf der Sphere) errechnen: (Projektion * ModelView * Position des aktuellen Vertex)
        // TODO: Eckpunkt ausgeben (siehe Methode S. 32 im Skript 4)

        gcolor = endColor;
        // TODO: gl_Position des Endvertex (Vertex am Ende der Startvertexnormale) errechnen: (Wie Startposition nur mit Addition des normalisiertem Normalenvektors auf die Position)
        // TODO: Eckpunkt ausgeben ( siehe Methode S.32 im Skript 4)

        EndPrimitive();
    }
}
