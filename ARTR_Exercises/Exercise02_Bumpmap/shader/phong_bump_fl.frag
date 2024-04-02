//Zunächst Definition der benötigten Datentypen
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 viewPosition;
    vec2 attenuations; // x = linearAttenuation, y = quadraticAttenuation
};

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

//Uniforms für Datenstrukturen zu Lichtquelle(n) und Materialien
uniform Material material;
uniform PointLight light;

//Lokale (veränderbare) Kopie des Materials
Material lMaterial = material;

//Texturen einbinden
uniform bool hastextureMap= false;
uniform sampler2D textureMap;
uniform bool hasbumpMap = false;
uniform sampler2D bumpMap;

//interpolierte Vertexattribute
in vec2 texCoords;
in vec3 viewPosition;
in vec3 viewNormal;

out vec4 fragColor;

//Im Folgenden Funktionen, die das Beleuchtungsmodell realisieren
//Es sind jeweils einzelne Funktionen, um eine einfachere Wiederverwendbarkeit zu schaffen
float phongKd(vec3 lightViewDirection, vec3 viewNormal)
{
    return max(dot(lightViewDirection, viewNormal), 0.0);
}

float phongKs(vec3 viewDirection, vec3 reflectDirection)
{
    return pow(max(dot(viewDirection, reflectDirection), 0.0), lMaterial.shininess);
}

float phongKa()
{
    return(1.0);
}

float computeLightAttenuation(float lightViewDistance, vec2 lightAttenuations)
{
    return 1.0 / (1.0 + lightAttenuations.x * lightViewDistance + lightAttenuations.y * lightViewDistance * lightViewDistance);
}

float computeMaterialAlpha()
{
    return clamp(lMaterial.emission.a + lMaterial.ambient.a + lMaterial.diffuse.a + lMaterial.specular.a, 0.0, 1.0);
}

vec3 phongKdKaColor(vec3 lightViewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, float lightAttenuation)
{
    vec3 diffuseColor = phongKd(lightViewDirection, viewNormal) * (lightDiffuse * lMaterial.diffuse.rgb);
    vec3 ambientColor = phongKa() * lightAmbient * lMaterial.ambient.rgb;
    vec3 color = (diffuseColor + ambientColor) * lightAttenuation;

    return color;
}

vec3 phongKdKaColorWithAttCalc(vec3 unnormalizedLightViewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, vec2 lightAttenuations)
{
    float lightViewDistance = length(unnormalizedLightViewDirection);
    float lightAttenuation = computeLightAttenuation(lightViewDistance, lightAttenuations);
    vec3 lightViewDirection = unnormalizedLightViewDirection / lightViewDistance;
    return phongKdKaColor(lightViewDirection, viewNormal, lightAmbient, lightDiffuse, lightAttenuation);
}

vec3 phongKsColor(vec3 lightViewDirection, vec3 viewDirection, vec3 viewNormal, vec3 lightSpecular, float lightAttenuation)
{
    //Vorsicht, GLSL reflect Funktion verlangt einfallenden Lichtvektor
    return (phongKs(viewDirection, reflect(-lightViewDirection, viewNormal)) * lightAttenuation * (lightSpecular * lMaterial.specular.rgb));
}

vec3 phong(vec3 unnormalizedLightViewDirection, vec3 viewDirection, vec3 viewNormal, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, vec2 lightAttenuations)
{
    float lightViewDistance = length(unnormalizedLightViewDirection);
    float lightAttenuation = computeLightAttenuation(lightViewDistance, lightAttenuations);
    vec3 lightViewDirection = normalize(unnormalizedLightViewDirection);
    vec3 specularColor = phongKsColor(lightViewDirection, viewDirection, viewNormal, lightSpecular, lightAttenuation);
    return phongKdKaColor(lightViewDirection, viewNormal, lightAmbient, lightDiffuse, lightAttenuation) + specularColor;
}

//Fuer das Verständnis der hier angewandten Berechnung des TBN-Systems sind folgende Quellen zu studieren:
//http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/ [1]
//und als Verbesserung/follow-up:
//http://www.thetenthplanet.de/archives/1180 [2]
//Noch lesenswert: Eigentlich müsste die Inverse einer Matrix berechnet werden
//Die Inverse kann generell durch die adjugierte Matrix/det berechnet werden
//Unter http://www.emptyloop.com/technotes/A%20tutorial%20on%20inverting%203%20by%203%20matrices%20with%20cross%20products.pdf
//ist die Berechnung der adjugierten Matrix durch Kreuzprodukte dargestellt
//Daraus laesst sich (siehe [2]) dann die ug. Berechnung des contangent_frame ableiten

//Berechnet die Matrix, die den gestörten Normalenvektor aus dem TBN-Koordinantensystem ins WKS umrechnet!
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and
    // V, the view vector (vertex to eye)
   vec3 map = texture(bumpMap, texcoord).xyz;
   //siehe sign expansion in [2]
   //Nächste Zeile auskommentieren für VZ-behaftete Maps
   map = map * 255./127. - 128./127.;
   //Nächste Zeile auskommentieren für Maps, die schon mit 3 Kanälen kommen - selten!
   map.z = sqrt( 1. - dot( map.xy, map.xy ) );
   mat3 TBN = cotangent_frame(N, -V, texcoord);
   return normalize(TBN * map);
}

void main()
{
    //Diffusen Anteil des Materials auf Farbe der diffusen Textur setzen
    lMaterial.diffuse = texture(textureMap, texCoords);

    //Durch Bumpmap angepasste Normale im WKS (!) berechnen
    vec3 perturbedViewNormal = perturb_normal(normalize(viewNormal), viewPosition, texCoords.xy);

    //Das Licht folgt hier der Kamera
    vec3 lightViewDirection;
    lightViewDirection = light.viewPosition - viewPosition;
    vec3 viewDirection = -normalize(viewPosition);
    //Um es an seinem Ort im Weltkoordinatensystem zu halten, muss die viewMatrix angewandt werden:
    lightViewDirection = vec3(viewMatrix*vec4(light.viewPosition,1.0)) - viewPosition;

    vec3 vertexColor = phong(lightViewDirection, viewDirection, perturbedViewNormal, light.ambient, light.diffuse, light.specular, light.attenuations);
    fragColor = vec4(vertexColor + material.emission.rgb, computeMaterialAlpha());
}
