uniform bool hastextureMap = false;
uniform samplerCube textureMap;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec3 eyeN;
in vec3 eyeVertex;
out vec4 fragColor;

void main()
{
    if(hastextureMap)
    {
        vec3 normEyeN = normalize(eyeN);
        vec3 normEyeVertex = normalize(eyeVertex);

        //Refraktion (Brechung) geht genau so, dann refract() verwenden
        vec3 eyeReflected = vec3(inverse(modelViewMatrix)*vec4(reflect(normEyeVertex, normEyeN),0.0));

        //Cubemaps kommen (meistens, so auch bei den Beispielmaps) aus rechtshändigen KS
        vec4 textureFrag = texture(textureMap, normalize(vec3(eyeReflected.x, -eyeReflected.yz)));
        fragColor = vec4(textureFrag.rgb, textureFrag.a);
//        fragColor = vec4(eyeReflected,1.0);
    }
    else
        fragColor = vec4(1.,0.,0.,1.);
}
