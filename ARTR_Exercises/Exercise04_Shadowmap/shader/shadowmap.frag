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
        float ambient = 0.15f;
        float shadowed = 0.25f;
        float notShadowed = 0.70;

        //hier bias einfügen
      
        fragColor.a = outputColor.a;

        //shadowCoord ist in [-1;1]x[-1;1]
        //für Texturzugriff in [0;1]x[0;1] umrechnen
        vec3 projCoords = shadowCoord.xyz/shadowCoord.w;
        projCoords = projCoords * 0.5 + 0.5;

        //hier Shadowmap Algrithmus einfügen

        //ambiente Beleuchtung
        fragColor.xyz += 0.4*outputColor.xyz;
    }
}
