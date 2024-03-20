in vec4 outputColor;
uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = outputColor;
}
