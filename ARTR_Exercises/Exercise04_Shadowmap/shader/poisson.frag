in vec4 outputColor;
in vec4 ShadowCoord;

uniform sampler2D textureMap4;
uniform bool buildingShadowmap;

layout (location = 0) out vec4 fragColor;

vec2 poissonDisk[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

void main(){

    if(buildingShadowmap){
        // nothing has to be done here

    }else{

        fragColor = outputColor;

        float bias = 0.005;

        int sampleAmount = 16;
        float visibility = 1;

        //shadowCoord ist in [-1;1]x[-1;1]
        //für Texturzugriff in [0;1]x[0;1] umrechnen
        vec3 projCoords = ShadowCoord.xyz/ShadowCoord.w;
        projCoords = projCoords * 0.5 + 0.5;

        //Hier PoissonDisk ALgorithmus einfügen

    fragColor.xyz = 1 * outputColor.xyz * visibility;
    }
}
