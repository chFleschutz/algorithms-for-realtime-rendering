
layout (location = 0) out vec4 fragColor;

in vec3 gNormal;
in vec4 gColor;
in float gHeight;

void main()
{
    fragColor = vec4(gHeight, gHeight, gHeight, 1.0);
}
