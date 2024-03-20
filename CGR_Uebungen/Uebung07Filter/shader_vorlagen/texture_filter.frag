uniform bool hastextureMap=false;
uniform sampler2D textureMap;
uniform float Time=1;

smooth in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec3 color = vec3(1.0,0.0,0.0);
        fragColor = mix(texture(textureMap, texCoords), vec4(color,1.0), Time);
    }
    else
        fragColor = vec4(Time,Time,1.0,1.0);

}
