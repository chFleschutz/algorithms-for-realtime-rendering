uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

layout (location = 0) in vec4 vertexPosition;

out vec3 Color;
uniform float Time;

void main()
{
    vec3 Schwerpunkt = vec3(0.0, 0.0, 0.0);
    vec3 sphereProj = vertexPosition.xyz-Schwerpunkt;
    vec3 vertex;
    vec3 normal;

    sphereProj = normalize(sphereProj);
    normal = sphereProj;
    vertex = mix(sphereProj, vertexPosition.xyz-Schwerpunkt, Time)+Schwerpunkt;

    Color = mix(vec3(1.0,0.0,0.0), vec3(0.0,1.0,1.0), Time);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex,1.0);
}
