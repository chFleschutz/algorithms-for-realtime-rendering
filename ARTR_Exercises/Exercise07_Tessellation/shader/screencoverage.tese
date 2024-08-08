
layout (triangles, equal_spacing, ccw) in;
in vec4 tcPosition[];

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

void main()
{
    // TODO siehe Skript Teil 4 Folie 58; Achtung: wir arbeiten mit vec4 anstelle von vec3; tePatchDistance benötigen wir hier nicht
    vec3 p0 = gl_TessCoord.x * tcPosition[0].xyz;
    vec3 p1 = gl_TessCoord.y * tcPosition[1].xyz;
    vec3 p2 = gl_TessCoord.z * tcPosition[2].xyz;
    vec3 tePosition = p0 + p1 + p2;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(tePosition, 1);
}
