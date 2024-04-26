#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ssao;
uniform bool redChannelOnly;

void main()
{             
    if (redChannelOnly)
		FragColor = vec4(texture(ssao, TexCoords).rrr, 1.0);
    else
        FragColor = vec4(texture(ssao, TexCoords).rgb, 1.0);
}
