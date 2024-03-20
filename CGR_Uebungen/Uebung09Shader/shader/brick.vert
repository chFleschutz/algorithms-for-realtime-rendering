// procedural brick texture
// inspired by ch. 6 in the orange book
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec4 vertexPosition;

out vec2 Pos_mc;   //Pos. in model coord.

void main()
{
    Pos_mc = vertexPosition.xy;
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;;
}
