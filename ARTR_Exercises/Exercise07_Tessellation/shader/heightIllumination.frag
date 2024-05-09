
layout (location = 0) out vec4 fragColor;

in vec4 vPosition;  // Die Position des Fragments im Weltkoordinatensystem
in vec3 gNormal;
in vec4 gColor;
in vec2 texCoord;
in float gHeight;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;
uniform mat3 normalMatrix;

vec4 lightPosition = vec4(-100.0f, 100.0f, -100.0f, 1.0f); // Position der Lichtquelle im Weltkoordinatensystem
vec3 lightColor    = vec3(1.0f);                           // Farbe des Lichts

uniform float waterLevel = 0.15;
uniform float sandLevel = 0.3f;
uniform float grassLevel = 0.5f;

const vec3 sandColor  = vec3(1.0,  0.98, 0.83);
const vec3 grassColor = vec3(0.44, 0.75, 0.28);
const vec3 rockColor  = vec3(0.33, 0.35, 0.36);

// Water / Sand / Grass levels
// https://github.com/Marculonis21/CPPDrawing/blob/main/terrainOpenGL/assets/shaders/noise.glsl
// Hash Funktion macht Pseudozufällige Zahlen
float hash(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

// TODO BONUSCHALLENGE: get_color Funktion um eine SchneeEbene erweitern.
// TODO EXTRABONUS: Diese soll über die Tasten 7 und 8 zur Laufzeit anpassbar sein. Dazu einmal in die tessplane.hpp schauen ;)
vec3 get_color(float height, vec2 uv)
{
    if (height <= waterLevel - 0.005)
    {
        return vec3(mix(vec3(0, 0, 0.4), sandColor, exp(-1.2 * waterLevel / height)));
    }

    height += (hash(uv) * 2 - 1) * 0.01;

    if (height < sandLevel)
    {
        return sandColor;
    }
    else if (height < grassLevel)
    {
        return grassColor;
    }

    return rockColor;
}

void main()
{
    vec3 viewPosition      = (modelViewMatrix * vPosition).xyz;
    vec3 viewLightPosition = (viewMatrix * lightPosition).xyz;
    // Normalisierung der Normalen
    vec3 norm = normalize(gNormal);
    // Richtung des Lichts berechnen
    vec3 lightDirection = normalize(viewLightPosition - viewPosition);
    // Lambert'sche Beleuchtung berechnen
    float diff = max(dot(norm, lightDirection), 0);
    // Endgültige Farbe berechnen
    vec3 albedoHeight = get_color(gHeight, texCoord); // height level coloring (Water/Sand/Grass/Rock)
//    vec3 albedoHeight = gColor.rgb; // texture coloring; alternative coloring (from texture set in myscene.cpp)
    vec3 diffuse = lightColor * diff * albedoHeight;
    fragColor = vec4(diffuse, 1.0);
}
