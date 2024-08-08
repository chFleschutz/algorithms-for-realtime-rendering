
layout(triangles, equal_spacing, ccw) in;
in vec4 tcPosition[];

uniform sampler2D textureMap;
uniform sampler2D heightMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

const float HEIGHT_SCALE = 10.0f;

in vec2 textureCoords[];
out vec4 vColor;
out vec2 texCoord;
out float height;

void main()
{
    // TODO: Bilineare Interpolation der Textur-Koordinaten; Die Textur-Koordinaten erhalten wir aus dem TCS hier im TES oben in "in vec2 textureCoords[]"
    // TODO siehe dazu: https://www.khronos.org/opengl/wiki/Tessellation#Triangles <- auch interessant für Generelles zur Tessellation
    texCoord = vec2(0.0f);


    vec4 p0, p1, p2, tePosition;
    // TODO: Lösung aus vorheriger Aufgabe einfügen


    // TODO: vColor und height aus textureMap bzw. heightMap auslesen; die Textur ist im fragment shader zunächst auskommentiert und ungenutzt, kann aber zum Testen einkommentiert werden


    // TODO: Höhe der tePosition ändern (z (!)) und in heightAdjustedTePosition speichern; height aus der heightMap nur hier (!) mit HEIGHT_SCALE multiplizieren
    vec4 heightAdjustedTePosition = tePosition; // <--
    gl_Position = vec4(tePosition.xy, tePosition.z + height * HEIGHT_SCALE, tePosition.w);
}
