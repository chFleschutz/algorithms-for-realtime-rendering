layout (location = 0) out vec3 textureMap0; //gPosition;
layout (location = 1) out vec3 textureMap1; //gNormal;
layout (location = 2) out vec3 textureMap2; //gAlbedo;

uniform bool hastextureMap = false;
uniform sampler2D textureMap;

uniform float UvelocityScale = .1;

out vec4 FragColor;

in vec2 texCoords;
in vec3 viewPosition;
in vec3 viewNormal;

in vec4 clipOldPosition; //nur xy und w
in vec4 clipNewPosition; //nur xy und w

void main()
{
    // store the fragment position vector in the first gbuffer texture
    textureMap0 = viewPosition;

    // hier soll die Geschwindigkeit berechnet werden (z kann dabei ignoriert werden)
    vec2 oldPos = clipOldPosition.xy;
    vec2 newPos = clipNewPosition.xy;
    vec2 lVelocity = (oldPos - newPos) * UvelocityScale;

    textureMap1 = vec3((lVelocity / max(length(lVelocity), 1.0) * 0.5 + 0.5), 1.0);
    // and the diffuse per-fragment color
    vec4 textureFrag = texture(textureMap, texCoords);
    textureMap2.rgb = textureFrag.rgb;
}
