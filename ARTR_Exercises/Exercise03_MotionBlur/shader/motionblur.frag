
uniform bool hastextureMap0 = false;
uniform bool hastextureMap1 = false;
uniform bool hastextureMap2 = false;
uniform sampler2D textureMap0; // position
uniform sampler2D textureMap1; // velocity
uniform sampler2D textureMap2; // color

const int samples = 8; 
const float blurStrength = 3.0;

in vec2 texCoords;
out vec4 fragColor;

vec3 motionblur(vec2 pixelCoord) 
{
    // see: https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect

    vec3 color = texture(textureMap2, pixelCoord).rgb;
    vec2 velocity = texture(textureMap1, pixelCoord).xy;
    vec2 uvStep = velocity * blurStrength;

    pixelCoord += uvStep;
    for(int i = 1; i < samples; i++)
	{
        pixelCoord += uvStep;
		color += texture(textureMap2, pixelCoord).rgb;
	}

    return color / float(samples);
}

void main()
{
    if(hastextureMap1)
    {
        vec4 textureFrag = texture(textureMap1, texCoords);
        fragColor = vec4(motionblur(texCoords), 1.0);
    }
    else //Um bei fehlender Textur überhaupt etwas zu sehen
        fragColor = vec4(texCoords.x,texCoords.y,0.,0.5);
}
