
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform float pSizeX=2.0;
uniform float pSizeY=2.0;

out vec2 texCoords;

const vec3 frontDir = vec3(0.0, 0.0, 1.0);
const vec3 upDir = vec3(0.0, 1.0, 0.0);
const float PI = 3.14159265359;
const float TAU = 6.28318530718;


void main()
{
    //Calculate View-Direction of Camera
    //which is the transformation of the z-axis
    //or the 3rd column of the MV-Matrix (but beware GLSL columnwise mats
    vec3 cameraRotation = normalize(viewMatrix[2].xyz); 
    
    const int atlasSize = 3;
    const int imposterCount = 8; // Number of imposters in the atlas (one spot is not used)
    vec2 imposterUVAtlas[atlasSize * atlasSize * 4] = vec2[atlasSize * atlasSize * 4](
        vec2(0.00, 0.66), vec2(0.00, 1.00), vec2(0.33, 0.66), vec2(0.33, 1.00), // Top Left
        vec2(0.33, 0.66), vec2(0.33, 1.00), vec2(0.66, 0.66), vec2(0.66, 1.00), // Top Middle
        vec2(0.66, 0.66), vec2(0.66, 1.00), vec2(1.00, 0.66), vec2(1.00, 1.00), // Top Right
        vec2(0.00, 0.33), vec2(0.00, 0.66), vec2(0.33, 0.33), vec2(0.33, 0.66), // Middle Left
        vec2(0.33, 0.33), vec2(0.33, 0.66), vec2(0.66, 0.33), vec2(0.66, 0.66), // Middle Middle
        vec2(0.66, 0.33), vec2(0.66, 0.66), vec2(1.00, 0.33), vec2(1.00, 0.66), // Middle Right
        vec2(0.00, 0.00), vec2(0.00, 0.33), vec2(0.33, 0.00), vec2(0.33, 0.33), // Bottom Left
        vec2(0.33, 0.00), vec2(0.33, 0.33), vec2(0.66, 0.00), vec2(0.66, 0.33), // Bottom Middle
        vec2(0.66, 0.00), vec2(0.66, 0.33), vec2(1.00, 0.00), vec2(1.00, 0.33) // Bottom Right
    );

    for (int i = 0; i < gl_in.length(); i++) 
    {
        vec3 position = gl_in[i].gl_Position.xyz;
        vec3 camDir = normalize(-position);
        vec3 tangent = normalize(cross(upDir, camDir));

        // Calculate the offsets for the positions
        vec3 rightOffset = 0.5 * pSizeX * tangent;
        vec3 upOffset = 0.5 * pSizeY * upDir;
        
        // Calculate the index of the imposter in the atlas
        float angleCamDir = atan(cameraRotation.z, cameraRotation.x); // Absolute angle between camera direction and x-axis
        float percent = (angleCamDir + PI) / TAU;                     // Percent of the circle
        int index = int(percent * float(imposterCount)) * 4;          // Index in the atlas array

        // Bottom Left
        gl_Position = projectionMatrix * vec4(position - rightOffset - upOffset, 1.0);
		texCoords = imposterUVAtlas[index + 0];
        EmitVertex();
        
        // Top Left
        gl_Position = projectionMatrix * vec4(position - rightOffset + upOffset, 1.0);
		texCoords = imposterUVAtlas[index + 1];
        EmitVertex();

        // Bottom Right
        gl_Position = projectionMatrix * vec4(position + rightOffset - upOffset, 1.0);
		texCoords = imposterUVAtlas[index + 2];
        EmitVertex();

        // Top Right
        gl_Position = projectionMatrix * vec4(position + rightOffset + upOffset, 1.0);
		texCoords = imposterUVAtlas[index + 3];
        EmitVertex();

        EndPrimitive();
    }
}
