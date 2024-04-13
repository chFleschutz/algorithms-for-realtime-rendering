
layout (location = 0) out vec3 textureMap0; // position
layout (location = 1) out vec3 textureMap1; // velocity
layout (location = 2) out vec3 textureMap2; // color

uniform bool hastextureMap = false;
uniform sampler2D textureMap;

uniform float UvelocityScale = 0.1;

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
    vec2 oldPos = clipOldPosition.xy / clipOldPosition.w;
    vec2 newPos = clipNewPosition.xy / clipNewPosition.w;
    vec2 velocity = (oldPos - newPos) * UvelocityScale;

    textureMap1 = vec3(velocity, 1.0) / 2.0f;
//    textureMap1 = vec3((lVelocity / max(length(lVelocity), 1.0) * 0.5 + 0.5), 1.0);

    // and the diffuse per-fragment color
    vec4 textureFrag = texture(textureMap, texCoords);
    textureMap2.rgb = textureFrag.rgb;
}


//// Get the depth buffer value at this pixel.    
//float zOverW = tex2D(depthTexture, texCoord); 
//
//// H is the viewport position at this pixel in the range -1 to 1.    
//float4 H = float4(texCoord.x * 2 - 1, (1 - texCoord.y) * 2 - 1, zOverW, 1); 
//
//// Transform by the view-projection inverse.    
//float4 D = mul(H, g_ViewProjectionInverseMatrix); 
//
//// Divide by w to get the world position.    
//float4 worldPos = D / D.w; 
//
//
//// Current viewport position    
//float4 currentPos = H; 
//
//// Use the world position, and transform by the previous view-projection matrix.    
//float4 previousPos = mul(worldPos, g_previousViewProjectionMatrix); 
//
//// Convert to nonhomogeneous points [-1,1] by dividing by w. 
//previousPos /= previousPos.w; 
//
//// Use this frame's position and last frame's to compute the pixel velocity.    
//float2 velocity = (currentPos - previousPos) / 2.0; 