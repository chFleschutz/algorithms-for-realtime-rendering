
layout (location = 0) out vec4 fragColor;

in vec4 vPosition;  // Die Position des Fragments im Weltkoordinatensystem
in vec3 gNormal;
in vec4 gColor;
in vec2 texCoord;
in float gHeight;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

vec4 lightPosition = vec4(-100.0, 100.0, -100.0, 1.0); // Position der Lichtquelle im Weltkoordinatensystem
vec3 lightColor    = vec3(1.0);                           // Farbe des Lichts
vec3 lightDirection = normalize(lightPosition.xyz);        // Richtung des Lichts

uniform float waterLevel = 0.15;
uniform float sandLevel = 0.3;
uniform float grassLevel = 0.5;
uniform float snowLevel = 0.7;

const vec3 sandColor  = vec3(1.0,  0.98, 0.83);
const vec3 grassColor = vec3(0.44, 0.75, 0.28);
const vec3 rockColor  = vec3(0.33, 0.35, 0.36);
const vec3 snowColor  = vec3(1.0,  1.0,  1.0);

// Water / Sand / Grass levels
// https://github.com/Marculonis21/CPPDrawing/blob/main/terrainOpenGL/assets/shaders/noise.glsl
// Hash Funktion macht Pseudozufällige Zahlen
float hash(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

// TODO BONUSCHALLENGE: get_color Funktion um eine SchneeEbene erweitern.
// TODO EXTRABONUS: Diese soll über die Tasten 7 und 8 zur Laufzeit anpassbar sein. Dazu einmal in die tessplane.hpp schauen ;)
vec3 terrainColor(float height, vec2 uv)
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
    else if (height < snowLevel)
	{
		return rockColor;
	}

    return snowColor;
}

void main()
{
    vec3 surfaceColor = terrainColor(gHeight, texCoord);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(gNormal);

    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * surfaceColor;
    vec3 ambient = vec3(0.1);

    fragColor = vec4(diffuse + ambient, 1.0);
}
