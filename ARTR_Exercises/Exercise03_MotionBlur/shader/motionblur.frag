uniform bool hastextureMap0 = false;
uniform bool hastextureMap1 = false;
uniform bool hastextureMap2 = false;
uniform sampler2D textureMap0; //gPosition;
uniform sampler2D textureMap1; //gNormal;
uniform sampler2D textureMap2; //gAlbedo;

uniform float Uvstep = 4.0/4.0; //Max. Radius div. durch Samplebreite in Schleife, ggf. anpassen
in vec2 texCoords;
out vec4 fragColor;


vec3 motionblur(vec2 pixelCoord) {
    vec3 color = texture(textureMap2, pixelCoord).xyz;
    vec2 velocity = texture(textureMap1, pixelCoord).xy * 2.0 - 1.0;

    //hier soll blurring über mehrere (z.B. 8) Samples implementiert werden
    //Tipp: ein Schleifendurchlauf deckt die Veränderung in sowie entgegen der Bewegungsrichtung ab

    return( color * 0.111111);
}
void main()
{
    if(hastextureMap1)
    {
        vec4 textureFrag = texture(textureMap1, texCoords);
        fragColor = vec4(textureFrag.rgb, 1.0);
        fragColor = vec4(motionblur(texCoords), 1.0);
    }
    else //Um bei fehlender Textur überhaupt etwas zu sehen
        fragColor = vec4(texCoords.x,texCoords.y,0.,0.5);
}
