layout (location = 0) out vec3 textureMap0; //gPosition;
layout (location = 1) out vec3 textureMap1; //gNormal;
layout (location = 2) out vec3 textureMap2; //gAlbedo;

out vec4 FragColor;

in vec2 texCoords;
in vec3 viewPosition;
in vec3 viewNormal;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    textureMap0 = viewPosition;
    // also store the per-fragment normals into the gbuffer
    textureMap1 = normalize(viewNormal);
    // and the diffuse per-fragment color
    textureMap2.rgb = vec3(0.95);
}
