uniform bool hastextureMap = false;
uniform sampler2D textureMap;

uniform float Time;

in vec2 texCoords;

out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, vec2(texCoords.x, texCoords.y+Time));
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
    }
    else //Um bei fehlender Textur überhaupt etwas zu sehen
        fragColor = vec4(texCoords.x,texCoords.y,0.,0.5);
}
