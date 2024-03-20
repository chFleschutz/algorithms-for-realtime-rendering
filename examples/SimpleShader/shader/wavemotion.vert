in vec3 vertexPosition;
in vec3 vertexColor;

uniform float Time;
uniform float xs=2.0, zs=10.0; // frequencies
uniform float h=1.0; // height scale

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform mat3 normalMatrix;

void main()
{
    float factorx = 1.;
    float factorz = 5.;
    float ltime = 2.*Time;
    vec4 t = vec4(vertexPosition,1.);
    t.y = vertexPosition.y + h*sin(3.141f*ltime + factorx*xs*vertexPosition.x) + h*sin(3.141f*ltime + factorz*zs*vertexPosition.z);
    gl_Position = projectionMatrix*modelViewMatrix*t;
}
