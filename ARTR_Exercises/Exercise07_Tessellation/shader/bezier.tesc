
uniform float uOuter0 = 1.0, uOuter1 = 64.0;

layout(vertices = 4) out;

void main()
{
    // See https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/tessellation.1pp.pdf
    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
    gl_TessLevelOuter[0] = float( uOuter0 );
    gl_TessLevelOuter[1] = float( uOuter1 );
}
