
layout (location = 0) out vec3 textureMap0; //gPosition;
layout (location = 1) out vec3 textureMap1; //gNormal;
layout (location = 2) out vec3 textureMap2; //gAlbedo;
uniform bool hastextureMap = false;
uniform sampler2D textureMap;

in vec2 texCoords;
in vec3 worldPos;
in vec3 worldNormal;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    textureMap0 = worldPos;
    // Normals in Textur speichern
    textureMap1 = worldNormal;
    // and the diffuse per-fragment color
    if(hastextureMap)
    {
        vec4 textureFrag = texture(textureMap, texCoords);
        textureMap2.rgb = textureFrag.rgb;
    } 
    else 
    {
        // Wenn keine Textur da ist irgendeine Farbe zuweisen
        textureMap2.rgb = vec3(0.5,1.0,0.1);
    }
}
