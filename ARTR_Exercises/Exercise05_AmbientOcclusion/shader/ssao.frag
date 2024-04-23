uniform sampler2D textureMap0; //textureMap0;
uniform sampler2D textureMap1; //textureMap1;
uniform sampler2D textureMap2; //gAlbedo;

uniform sampler2D SSAONoise; //Noise

uniform vec3 uSSAOKernel[64];

out float FragColor;

in vec2 texCoords;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1024.0/4.0, 1024.0/4.0);

uniform mat4 projectionMatrix;

void main()
{
    // zum debuggen nutzbar
    // FragColor = vec4(texCoords,0.0,1.0);
    // FragColor = texture(textureMap0, texCoords);
    // FragColor = texture(textureMap1, texCoords);
     FragColor = (texture(textureMap2, texCoords)).r; // TODO mich bitte wieder zum Schluss auskommentieren. Danke :)
    // FragColor = vec4(texture(SSAONoise, texCoords).xyz, 1.0);

    // TODO get input for SSAO algorithm


    // TODO create TBN change-of-basis matrix: from tangent-space to view-space


    // TODO iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // TODO get sample position


        // TODO project sample position (to sample texture) (to get position on screen/texture)


        // TODO get sample depth


        // TODO range check & accumulate


    }
    // TODO occlusion normalisieren mit der kernel größe

//    FragColor = occlusion;
}
