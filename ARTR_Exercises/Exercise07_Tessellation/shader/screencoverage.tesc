
layout(vertices = 3) out;

out vec4 tcPosition[];
in vec4 vPosition[];

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;
mat4 mvpMatrix = projectionMatrix * modelViewMatrix;

vec2 screen_size = vec2(800, 600);
float lod_factor = 50.0;

vec4 project(vec4 vertex)
{
    // TODO result aus projection und modelView matrizen zusammen mit vertex berechnen. Teilen durch w nicht vergessen!
    return (mvpMatrix * vertex) / vertex.w;
}

// Nochmal als Übersicht für die verschiedenen Spaces: https://learnopengl.com/Getting-started/Coordinate-Systems
vec2 screen_space(vec4 vertex)
{
    //1.3 statt 1, um geclippte Tris zu berücksichtigen
    return (clamp(vertex.xy, -1.3, 1.3) + 1.0) * (screen_size * 0.5);
}

float level(vec2 v0, vec2 v1)
{
    // TODO Distanz berechnen (mit der distance() Funktion)
    float dis = distance(v0, v1);
    return clamp(dis / lod_factor, 1, 64);
}

void main()
{
     tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
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
