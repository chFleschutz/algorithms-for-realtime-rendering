layout (triangles) in;
layout (triangle_strip) out; //es wird kein echter
layout (max_vertices = 10) out; //Strip erzeugt!

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;

in vec4 vColor[];
in float height[];
out vec3 gNormal;
out vec4 gColor;
out float gHeight;

void main(void)
{
    vec3 v0 = vec3(normalize(gl_in[2].gl_Position - gl_in[0].gl_Position));
    vec3 v1 = vec3(normalize(gl_in[2].gl_Position - gl_in[1].gl_Position));
    gNormal = normalMatrix * vec3(cross(v0, v1));

    for (int i = 0; i < gl_in.length(); ++i)
    {
        gl_Position = projectionMatrix * modelViewMatrix * gl_in[i].gl_Position;
        gColor  = vColor[i];
        gHeight = height[i];

        EmitVertex();
    }

    EndPrimitive();
}
