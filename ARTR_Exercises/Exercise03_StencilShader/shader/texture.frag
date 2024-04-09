uniform bool hastextureMap= false;
uniform sampler2D textureMap;

in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, texCoords);
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
    }
    else //Um bei fehlender Textur überhaupt etwas zu sehen
        fragColor = vec4(1.0,1.0,1.0,1.0);
}
