uniform bool hastextureMap0 = false;
uniform sampler2D textureMap0;
uniform bool hastextureMap4 = false;
uniform sampler2D textureMap4;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap0 && hastextureMap4)
    {
        //Hole Farbwert aus Maske
        vec4 maskColor = texture(textureMap0, texCoords);

        if (maskColor.a < 0.1)
            discard;

        fragColor = texture(textureMap4, texCoords);
    }
    else
        fragColor = vec4(texCoords, 1.0, 1.0);
}
