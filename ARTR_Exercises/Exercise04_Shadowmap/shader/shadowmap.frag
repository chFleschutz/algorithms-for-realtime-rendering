
uniform bool hastextureMap4 = false;
uniform sampler2D textureMap4;

uniform bool buildingShadowmap;

in vec4 outputColor;
in vec4 shadowCoord;
in vec3 lightDir;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

//Bei perspektivischer Proj. ist der Depthbuffer nicht linear und kann deswegen nicht "gut"
//mit Grauwerten werden. Diese Funktion linearisiert die Werte wieder mit der Proj.-Matrix
float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 1000.0; // camera z far
  float z = texture(textureMap4, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));
}

void main()
{
    if(buildingShadowmap)
    {
        // nichts zu tun
    }
    else
    {
        float ambient = 0.2;
        float diffuse = 1.0 - ambient;

        float shadowed = 0.2;

        float lightAngle = dot(normalize(normal), normalize(lightDir));

        //hier bias einfügen
//        float bias = 0.005 * tan(acos(lightAngle));
//        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
        float bias = 0.001;

        //shadowCoord ist in [-1;1]x[-1;1]
        //für Texturzugriff in [0;1]x[0;1] umrechnen
        vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
        projCoords = projCoords * 0.5 + 0.5;

        // Shadow 
        float shadowZ = texture(textureMap4, projCoords.xy).x;
        float fragZ = projCoords.z;

        bool isShadowed = fragZ > shadowZ + bias;
        bool outOfMap = projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0;
        float shadowFactor = isShadowed && !outOfMap ? shadowed : 1.0;

        // Beleuchtung
        diffuse = diffuse * max(lightAngle, 0.0);
        diffuse = min(diffuse, shadowFactor);

        vec3 ambientColor = outputColor.rgb * ambient;
        vec3 diffuseColor = outputColor.rgb * diffuse;
        fragColor.rgb = ambientColor + diffuseColor;
        fragColor.a = outputColor.a;
    }
}
