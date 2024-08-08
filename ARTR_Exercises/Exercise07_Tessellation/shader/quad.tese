
layout (quads, equal_spacing, ccw) in;

uniform mat4 modelMatrix;           
uniform mat4 viewMatrix;           
uniform mat4 projectionMatrix;      

in vec4 tcPosition[];

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = tcPosition[0];
    vec4 p01 = tcPosition[1];
    vec4 p10 = tcPosition[2];
    vec4 p11 = tcPosition[3];

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * p;
}