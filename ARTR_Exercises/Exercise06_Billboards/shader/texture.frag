uniform bool hastextureMap = false;
uniform sampler2D textureMap;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, texCoords);
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
        //Discard, falls transparente Textur andere Objekte verdecken würde
        if(textureFrag.a < 0.5)
            discard;
        
    }
    else
        fragColor = vec4(texCoords.x,texCoords.y,0.,1.);
}
