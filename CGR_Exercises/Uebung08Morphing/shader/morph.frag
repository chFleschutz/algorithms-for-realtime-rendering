in vec3 Color;
out vec4 fragColor;

uniform float Time;

void main()
{
    fragColor = vec4(Color,1.0);
}
