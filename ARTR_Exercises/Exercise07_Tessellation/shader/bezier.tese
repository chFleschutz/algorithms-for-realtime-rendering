
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

layout(isolines, equal_spacing) in;

void main()
{
    // See https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/tessellation.1pp.pdf
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;
    float u = gl_TessCoord.x;

    // Bezier function
    float b0 = (1.-u) * (1.-u) * (1.-u);
    float b1 = 3. * u * (1.-u) * (1.-u);
    float b2 = 3. * u * u * (1.-u);
    float b3 = u * u * u;

    vec4 position = vec4(gl_TessCoord.y + b0*p0 + b1*p1 + b2*p2 + b3*p3);
    gl_Position = projectionMatrix * modelViewMatrix * position;
}

