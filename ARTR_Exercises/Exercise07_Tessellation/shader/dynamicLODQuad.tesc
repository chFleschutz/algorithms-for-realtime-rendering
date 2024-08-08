
layout(vertices = 4) out;

uniform mat4 modelMatrix;           
uniform mat4 viewMatrix;           
uniform mat4 projectionMatrix;      

in vec4 vPosition[];
in vec2 vTexCoord[];

out vec4 tcPosition[];
out vec2 tcTexCoord[];

void main()
{
    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
    if (gl_InvocationID == 0) 
    {
        const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 10;
        const float MAX_DISTANCE = 200;

        // Transform each vertex into eye space
        vec4 eyeSpacePos00 = viewMatrix * modelMatrix * vPosition[0];
        vec4 eyeSpacePos01 = viewMatrix * modelMatrix * vPosition[1];
        vec4 eyeSpacePos10 = viewMatrix * modelMatrix * vPosition[2];
        vec4 eyeSpacePos11 = viewMatrix * modelMatrix * vPosition[3];

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp((abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        // Interpolate edge tessellation level based on closer vertex
        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

        // Set the corresponding outer edge tessellation levels
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        // Set the inner tessellation levels to the max of the two parallel edges
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}
