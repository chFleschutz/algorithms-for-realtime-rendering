
uniform bool hastextureMap0 = false;
uniform bool hastextureMap1 = false;
uniform bool hastextureMap2 = false;
uniform sampler2D textureMap0; //gPosition;
uniform sampler2D textureMap1; //gNormals;
uniform sampler2D textureMap2; //gAlbedo;

uniform vec3 viewPos;
uniform int actualLightsNumber;
// Variablen für Lichter
struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPosition;
    vec2 attenuations; 
};

const int MAX_NR_LIGHTS = 100;
uniform PointLight lights[MAX_NR_LIGHTS];

in vec2 texCoords;

out vec4 fragColor;

void main()
{            
    // retrieve data from G-buffer
    vec3 FragPos = texture(textureMap0, texCoords).rgb;
    vec3 Normal = texture(textureMap1, texCoords).rgb;
    vec3 Albedo = texture(textureMap2, texCoords).rgb;
    
     
    // TODO: Hier Lichtberechnung einbauen
    
    for(int i = 0; i < actualLightsNumber; ++i)
    {
        // TODO: Hier Radius und Lichtabstandsberechnung eventuell einbauen
//        if(distance < radius) {
//            // Attenuation: Beeinflusst wie stark die Stärke des Lichts auf Distanz abfällt
              // Mit den Werten von Linear und Quadratic rumexperimentieren ;)
//            float attenuation = 1.0/(constant + linear * distance + quadratic * distance * distance);
//            diffuse *= attenuation;
//            
//            lighting += diffuse;
//        }
    }
    
    // TODO: Albedo durch berechnete Farbe ersetzen
    fragColor = vec4(Albedo, 1.0);
}  

