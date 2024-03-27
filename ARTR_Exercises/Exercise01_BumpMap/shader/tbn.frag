uniform float bumpGridSize = 32.0;
uniform float bumpRadius = 0.4;
uniform vec4 color = vec4(1.0, 0.5, 0.0, 1.0);

in vec2 texCoords;
in vec3 tangentLightDirection;
in vec3 tangentCamDirection;

out vec4 fragColor;

void main()
{
    vec3 tangentLightDir = normalize(tangentLightDirection);
    vec3 tangentCamDir = normalize(tangentCamDirection);
    vec3 tangentNormal = vec3(0.0, 0.0, 1.0);

    // bump mapping
    vec2 pos = texCoords * bumpGridSize;
    vec2 gridPos = fract(pos) - vec2(0.5, 0.5);

    float radiusSquared = bumpRadius * bumpRadius;
    float distSquared = dot(gridPos, gridPos);
    
    if (distSquared < radiusSquared)
	{
		float height = sqrt(radiusSquared - distSquared);
		tangentNormal = normalize(vec3(gridPos, height));
	}

    // perform illumination
    vec3 ambientColor = 0.3 * color.rgb;

    float diffuseFactor = 0.6 * max(dot(tangentNormal, tangentLightDir), 0.0);
    vec3 diffuseColor = diffuseFactor * color.rgb;
	
    vec3 reflectDir = reflect(-tangentLightDir, tangentNormal);
    float shininess = 128;
    float specularFactor = 0.0;
    if(diffuseFactor > 0.0)
        specularFactor = pow(max(dot(tangentCamDir, reflectDir), 0.0), shininess);
    vec3 specularColor = specularFactor * color.rgb;

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
