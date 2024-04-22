
in vec4 outputColor;
in vec4 ShadowCoord;

uniform sampler2D textureMap4;
uniform bool buildingShadowmap;

layout (location = 0) out vec4 fragColor;

const int sam = 2;
const float tests = (sam * 2.0 + 1.0) * (sam * 2.0 + 1.0);

void main(){

    if (buildingShadowmap)
    {
        // nothing has to be done here
    }
    else 
    {
        fragColor = outputColor;

        float bias = 0.01;
        float counter = 0.0;

        vec3 projCoords = ShadowCoord.xyz/ShadowCoord.w;
        projCoords = projCoords * 0.5 + 0.5;

        //Hier PCF Algorithmus eingügen

        float shadow = 1.0 - (counter * ShadowCoord.w);

        fragColor.xyz = 1 * outputColor.xyz * shadow;
    }
}
