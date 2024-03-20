uniform bool hastextureMap3 = false;
uniform samplerCube textureMap3;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec3 eyeN;
in vec3 eyeVertex;
in vec3 temptc;
in vec3 refl;
out vec4 fragColor;

void main()
{
    if(hastextureMap3)
    {
        vec3 normEyeN = normalize(eyeN);
        vec3 normEyeVertex = normalize(eyeVertex);

        //Refraktion (Brechung) geht genau so, dann refract() verwenden
        vec3 eyeReflected = vec3(inverse(modelViewMatrix)*vec4(reflect(normEyeVertex, normEyeN),0.0));

        //zum Zugriff auf SamplerCube siehe GLSpec 4.5 Core, Seite 240
        vec4 textureFrag = texture(textureMap3, eyeReflected);
        textureFrag = texture(textureMap3, normalize(vec3(eyeReflected.x, -eyeReflected.yz)));
//        textureFrag = texture(textureMap0, temptc);
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
    }
    else
        fragColor = vec4(1.,0.,0.,1.);
}
