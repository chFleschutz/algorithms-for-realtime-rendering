
uniform sampler2D textureMap0; //textureMap0;
uniform sampler2D textureMap1; //textureMap1;
uniform sampler2D textureMap2; //gAlbedo;

uniform sampler2D SSAONoise; //Noise

uniform vec3 uSSAOKernel[64];

uniform mat4 projectionMatrix;

out float fragColor;

in vec2 texCoords;

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1024.0 / 4.0, 1024.0 / 4.0);

void main()
{
    // Inputs from gbuffer
    vec3 position = texture(textureMap0, texCoords).xyz;
    vec3 normal = texture(textureMap1, texCoords).xyz;
    vec3 albedo = texture(textureMap2, texCoords).xyz;
    vec3 randomVec = texture(SSAONoise, texCoords * noiseScale).xyz;

    // TBN matrix
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // SSAO calculation
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
	{
		vec3 samplePos = TBN * uSSAOKernel[i]; // from tangent to view-space
        samplePos = position + samplePos * radius;

		vec4 offset = vec4(samplePos, 1.0);
		offset = projectionMatrix * offset;     // view to clip-space
		offset.xyz /= offset.w;                 // normalize by w
		offset.xyz = offset.xyz * 0.5 + 0.5;    // range from [-1, 1] to [0, 1]

		float sampleDepth = texture(textureMap0, offset.xy).z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
    occlusion /= kernelSize;

    // Color calculation
    float ambient = 1.0 - occlusion;
    fragColor = ambient;

    // fragColor = vec4(texCoords,0.0,1.0);
    // fragColor = texture(textureMap0, texCoords);
    // fragColor = texture(textureMap1, texCoords);
    // fragColor = (texture(textureMap2, texCoords)).r; // TODO mich bitte wieder zum Schluss auskommentieren. Danke :)
    // fragColor = vec4(texture(SSAONoise, texCoords).xyz, 1.0);

}
