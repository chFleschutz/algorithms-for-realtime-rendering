
layout (quads, equal_spacing, ccw) in;

uniform sampler2D heightMap;  
uniform mat4 modelMatrix;           
uniform mat4 viewMatrix;           
uniform mat4 projectionMatrix;      

in vec4 tcPosition[];
in vec2 tcTexCoord[];

out vec4 vColor;
out vec2 texCoord;
out float height;

const float heightScale = 10.0;
const float heightShift = -5.0;

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // ----------------------------------------------------------------------
    // retrieve control point texture coordinates
    vec2 t00 = tcTexCoord[0];
    vec2 t01 = tcTexCoord[1];
    vec2 t10 = tcTexCoord[2];
    vec2 t11 = tcTexCoord[3];

    // bilinearly interpolate texture coordinate across patch
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    texCoord = (t1 - t0) * v + t0;

    // lookup texel at patch coordinate for height and scale + shift as desired
    height = texture(heightMap, texCoord).y;

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = tcPosition[0];
    vec4 p01 = tcPosition[1];
    vec4 p10 = tcPosition[2];
    vec4 p11 = tcPosition[3];

    // compute patch surface normal
    vec4 uVec = p10 - p00;
    vec4 vVec = p01 - p00;
    vec3 normal = normalize(cross(vVec.xyz, uVec.xyz));

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // displace point along normal
    p += vec4(normal, 0.0) * (height * heightScale + heightShift);

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = p;

    vColor = vec4(1.0, 1.0, 1.0, 1.0);
}