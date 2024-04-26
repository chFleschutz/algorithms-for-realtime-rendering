
uniform bool hastextureMap4 = false;
uniform sampler2D textureMap4;

uniform bool buildingShadowmap;
uniform int softShadowMode;

in vec4 outputColor;
in vec4 shadowCoord;
in vec3 lightDir;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

const int shadowMapSize = 1024;
const float softShadowSize = 2.0;

const float ambient = 0.2;
const float diffuse = 1.0 - ambient;
const float shadowed = 0.2;

float shadow(vec2 uv, float fragZ);
float shadowPCF(vec2 uv, float fragZ);
float shadowPoissonDisk(vec2 uv, float fragZ);

void main()
{
    if(buildingShadowmap)
    {
        // nichts zu tun
    }
    else
    {
        float lightAngle = dot(normalize(normal), normalize(lightDir));

        // Bias
        float bias = 0.005 * tan(acos(lightAngle));
//        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
//        float bias = 0.001;

        // Normalize and transform shadowCoord to [0,1]
        vec2 shadowUV = (shadowCoord.xy / shadowCoord.w) * 0.5 + 0.5;
        float fragDepth = ((shadowCoord.z - bias) / shadowCoord.w) * 0.5 + 0.5;

        // Shadow 
        float shadowFactor = 0.0;
        if (softShadowMode == 1)
		{
			shadowFactor = shadowPCF(shadowUV, fragDepth);
		}
		else if (softShadowMode == 2)
		{
			shadowFactor = shadowPoissonDisk(shadowUV, fragDepth);
		}
		else // default = 0
		{
			shadowFactor = shadow(shadowUV, fragDepth);
		}

        // Beleuchtung
        float diffuseFactor = diffuse * max(lightAngle, 0.0);
        diffuseFactor = min(diffuseFactor, shadowFactor);

        vec3 ambientColor = outputColor.rgb * ambient;
        vec3 diffuseColor = outputColor.rgb * diffuseFactor;
        fragColor.rgb = ambientColor + diffuseColor;
        fragColor.a = outputColor.a;
    }
}

float shadow(vec2 uv, float fragZ)
{
    float shadowZ = texture(textureMap4, uv).x;

    bool isShadowed = fragZ > shadowZ;
    bool outOfMap = uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0;
    return isShadowed && !outOfMap ? shadowed : 1.0;
}

float shadowPCF(vec2 uv, float fragZ)
{
    float uvStep = (1.0 / float(shadowMapSize)) * softShadowSize;
    float shadow = 0.0;
    
    // 3x3 PCF
    for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
            vec2 shadowUV = uv + (vec2(x, y) * uvStep);
            float shadowZ = texture(textureMap4, shadowUV).x;

            bool isShadowed = fragZ > shadowZ;
            bool outOfMap = shadowUV.x < 0.0 || shadowUV.x > 1.0 || shadowUV.y < 0.0 || shadowUV.y > 1.0;
            shadow += isShadowed && !outOfMap ? shadowed : 1.0;
		}
	}

    return shadow / 9.0;
}

float shadowPoissonDisk(vec2 uv, float fragZ)
{
	float uvStep = (1.0 / float(shadowMapSize)) * softShadowSize;
    float shadow = 0.0;

    vec2 poissonDisk[16] = vec2[](
        vec2(0.0289919, 0.300469),
        vec2(0.972793, 0.115546),
        vec2(0.452908, 0.0353056),
        vec2(0.67248, 0.646939),
        vec2(0.0420571, 0.728857),
        vec2(0.0922788, 0.0604377),
        vec2(0.308612, 0.638384),
        vec2(0.987422, 0.878242),
        vec2(0.721617, 0.0124422),
        vec2(0.768715, 0.263133),
        vec2(0.367515, 0.290535),
        vec2(0.895988, 0.595451),
        vec2(0.379056, 0.998533),
        vec2(0.0929502, 0.957188),
        vec2(0.599921, 0.893207),
        vec2(0.571916, 0.429496)
    );

	for (int i = 0; i < 16; i++)
	{
        vec2 offset = ((2.0 * poissonDisk[i]) - 1.0) * uvStep;
		vec2 shadowUV = uv + offset;
		float shadowZ = texture(textureMap4, shadowUV).x;

		bool isShadowed = fragZ > shadowZ;
		bool outOfMap = shadowUV.x < 0.0 || shadowUV.x > 1.0 || shadowUV.y < 0.0 || shadowUV.y > 1.0;
		shadow += isShadowed && !outOfMap ? shadowed : 1.0;
	}

	return shadow / 16.0;
}
