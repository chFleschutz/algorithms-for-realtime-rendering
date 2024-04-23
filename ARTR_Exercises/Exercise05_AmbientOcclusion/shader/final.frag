uniform bool hastextureMap0 = false;
uniform sampler2D textureMap0;
uniform bool blur = false;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if (blur) {
        vec2 texelSize = 1.0 / vec2(textureSize(textureMap0, 0));
        float result = 0.0;
        for (int x = -2; x < 2; ++x)
        {
            for (int y = -2; y < 2; ++y)
            {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                result += texture(textureMap0, texCoords + offset).r;
            }
        }
        float blurred = result / (4.0 * 4.0);
        fragColor = vec4(vec3(blurred), 1.0);

    }
    else
        fragColor = vec4(texture(textureMap0, texCoords).rrr, 1.0);
}
