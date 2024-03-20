uniform bool hastextureMap = false;
uniform samplerCube textureMap;

in vec3 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, texCoords);
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
    }
    else //Um bei fehlender Textur überhaupt etwas zu sehen
        fragColor = vec4(texCoords.x,texCoords.y,0.,0.5);
}
