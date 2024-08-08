
layout(vertices = 3) out;

out vec4 tcPosition[];
in vec4 vPosition[];

uniform float innerTessLevel;
uniform float outerTessLevel;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix * modelMatrix;
mat4 mvpMatrix = projectionMatrix * modelViewMatrix;

void main()
{
    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    if (gl_InvocationID == 0) 
    {
        const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 32;
        const float MIN_DISTANCE = 10;
        const float MAX_DISTANCE = 200;

        // Tranform vertex position into eye space
        vec4 eyeSpacePos0 = viewMatrix * modelMatrix * vPosition[0];
        vec4 eyeSpacePos1 = viewMatrix * modelMatrix * vPosition[1];
        vec4 eyeSpacePos2 = viewMatrix * modelMatrix * vPosition[2];

        // Compute distance to camera between [0, 1]
        float distance0 = clamp((abs(eyeSpacePos0.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance1 = clamp((abs(eyeSpacePos1.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance2 = clamp((abs(eyeSpacePos2.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        // Compute tessellation level
        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, distance0);
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, distance1);
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, distance2);

        // Set tessellation levels
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel2;
        gl_TessLevelOuter[2] = tessLevel1;

        gl_TessLevelInner[0] = max(max(tessLevel0, tessLevel1), tessLevel2);
    }
}
