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
    // Display normals for each vertex
    for (int i = 0; i < 1; i++) 
    {
        gcolor = startColor;
        gl_Position = projectionMatrix * modelViewMatrix * vPosition[i];
        EmitVertex();

        gcolor = endColor;
        vec4 endPos = vPosition[i] + vec4(normalize(vnormal[i]), 0.0);
        gl_Position = projectionMatrix * modelViewMatrix * endPos;
        EmitVertex();

        EndPrimitive();
    }
}
