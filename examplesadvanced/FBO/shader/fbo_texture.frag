uniform bool hastextureMap3 = false;
uniform sampler2D textureMap3;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap3)
    {
        vec4 textureFrag = texture(textureMap3, texCoords);
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
    }
    else
        fragColor = vec4(texCoords.x,texCoords.y,0.,1.);
}
