
layout(vertices = 3) out;

out vec4 tcPosition[];
out vec2 textureCoords[];

in vec4 vPosition[];
in vec2 texCoords[];

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

vec2 screen_size = vec2(800,600);
float lod_factor = 500.0;

vec4 project(vec4 vertex)
{
    // TODO Lösung aus vorheriger Aufgabe hier einfügen!
    vec4 result = vertex; // <-- hier
    return result;
}

vec2 screen_space(vec4 vertex)
{
    //1.3 statt 1, um geclippte Tris zu berücksichtigen
    return (clamp(vertex.xy, -1.3, 1.3)+1) * (screen_size*0.5);
}

float level(vec2 v0, vec2 v1)
{
    // TODO Lösung aus vorheriger Aufgabe hier einfügen!
    float distance = 0.0f; // <--

    // TODO ACHTUNG: andere Berechnung als in vorheriger Aufgabe. Kann einfach so stehen bleiben!
    return clamp(pow(distance, 1.5f)/lod_factor, 1, 64);
}

void main()
{
     tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
     // TODO Textur-Koordinaten durchreichen


     if (gl_InvocationID == 0)
     {
         vec4 v0 = project(vPosition[0]);
         vec4 v1 = project(vPosition[1]);
         vec4 v2 = project(vPosition[2]);
         vec2 ss0 = screen_space(v0);
         vec2 ss1 = screen_space(v1);
         vec2 ss2 = screen_space(v2);

         float e0 = level(ss1, ss2);
         float e1 = level(ss2, ss0);
         float e2 = level(ss0, ss1);

         gl_TessLevelInner[0] = mix(e1, e2, 0.5);
         gl_TessLevelOuter[0] = e0;
         gl_TessLevelOuter[1] = e1;
         gl_TessLevelOuter[2] = e2;
     }
}
