
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 projectionMatrix;
uniform float pSizeX = 2.5;
uniform float pSizeY = 3.5;

out vec2 texCoords;

const vec3 upDir = vec3(0.0, 1.0, 0.0);

void main()
{
    for (int i = 0; i < gl_in.length(); i++) 
    {
        vec3 position = gl_in[i].gl_Position.xyz;
        vec3 camDir = normalize(-position);
        vec3 tangent = normalize(cross(upDir, camDir));

        vec3 leftOffset = 0.5 * pSizeX * tangent;
        vec3 upOffset = 0.5 * pSizeY * upDir;
        
        // Bottom Left
        gl_Position = projectionMatrix * vec4(position + leftOffset - upOffset, 1.0);
        texCoords = vec2(0, 0);
        EmitVertex();
        
        // Bottom Right
        gl_Position = projectionMatrix * vec4(position - leftOffset - upOffset, 1.0);
        texCoords = vec2(1, 0);
        EmitVertex();

        // Top Left
        gl_Position = projectionMatrix * vec4(position + leftOffset + upOffset, 1.0);
        texCoords = vec2(0, 1);
        EmitVertex();

        // Top Right
        gl_Position = projectionMatrix * vec4(position - leftOffset + upOffset, 1.0);
        texCoords = vec2(1, 1);
        EmitVertex();

        EndPrimitive();
    }
}
