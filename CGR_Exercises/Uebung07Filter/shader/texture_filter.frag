uniform bool hastextureMap= false;
uniform sampler2D textureMap;
uniform float Time=1;

smooth in vec2 texCoords;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        mat3 laplace1 = mat3(0.0,1.0,0.0,1.0,-4.0,1.0,0.0,1.0,0.0);
        mat3 laplace2 = mat3(1.0,1.0,1.0,1.0,-8.0,1.0,1.0,1.0,1.0);
        mat3 gauss = mat3(1.0,2.0,1.0,2.0,4.0,2.0,1.0,2.0,1.0);
        mat3 emboss = mat3(2.0,0.0,0.0,0.0,-1.0,0.0,0.0,0.0,-1.0);
        mat3 theKernel;
        vec3 color = vec3(0.0,0.0,0.0);
        ivec2 tsize = textureSize(textureMap, 0);
        float stepX = 1.0/tsize.x;
        float stepY = 1.0/tsize.y;

            theKernel = gauss/8.0;
    //    theKernel = laplace2;
            theKernel = laplace1;
    //        theKernel = emboss;

        if (texCoords.y < 0.5)
                for (int i=-1; i<2; i++)
                        for (int j=-1; j<2; j++)
                        {
                                color += float(theKernel[i+1][j+1])*vec3(texture(textureMap, vec2(texCoords.x + float(i)*stepX,texCoords.y + float(j)*stepY)));
                        }
        else {
                color = vec3(texture(textureMap,texCoords));
        }

        fragColor = mix(texture(textureMap, texCoords), vec4(color,1.0), Time);
    }
    else
        fragColor = vec4(Time,Time,1.0,1.0);
}
