
uniform bool hastextureMap = false;
uniform sampler2D textureMap;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, texCoords);

        // Discard if the fragment is transparent
        if(textureFrag.a < 0.5)
            discard;
        
        fragColor = textureFrag;
    }
    else
    {
        fragColor = vec4(texCoords.x,texCoords.y,0.,1.);
    }
}
