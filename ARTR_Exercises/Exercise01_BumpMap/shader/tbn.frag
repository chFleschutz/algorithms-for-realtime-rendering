uniform float bumpGridSize = 32.0f;
uniform float bumpRadius = 0.4f;
uniform vec4 color = vec4(1.0f,0.5f,0.0f,1.0f);

in vec2 texCoords;
in vec3 tangentLightDirection;
in vec3 tangentCamDirection;

out vec4 fragColor;

void main()
{
    vec3 tangentLightDir = normalize(tangentLightDirection);
    vec3 tangentCamDir = normalize(tangentCamDirection);
    vec3 tangentNormal = vec3(0.f, 0.f, 1.f);

    // An dieser Stelle soll mithilfe der uniformen Variablen "bumpGridSize" und "bumpRadius" ein Fragment-Shader erstellt werden,
    // welcher durch die Veränderung der Werte von "tangentNormal" unserer Ebene ein Raster aus schattierten Bumpmap-Halbkugeln verleiht.

    // perform illumination
    vec3 ambientColor = 0.3f * color.rgb;

    float diffuseFactor = 0.6f * max(dot(tangentNormal, tangentLightDir), 0.0);
    vec3 diffuseColor = diffuseFactor * color.rgb;
	
    vec3 reflectDir = reflect(-tangentLightDir, tangentNormal);
    float shininess = 128;
    float specularFactor = 0.f;
    if(diffuseFactor > 0.f)
        specularFactor = pow(max(dot(tangentCamDir, reflectDir), 0.0), shininess);
    vec3 specularColor = specularFactor * color.rgb;

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
