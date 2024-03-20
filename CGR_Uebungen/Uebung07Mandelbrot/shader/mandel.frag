out vec4 fColor;
uniform float MaxIterations=1000;
uniform float Zoom=1.0;
uniform vec2  Center=vec2(0.0,0.0);
uniform vec3  InnerColor=vec3(1.0,0.0,0.0);
uniform vec3  OuterColor1=vec3(0.0,1.0,0.0);
uniform vec3  OuterColor2=vec3(0.0,0.0,1.0);

in vec2 texCoords;

void main()
{
   fColor = vec4(mix(OuterColor1, OuterColor2, texCoords.x),1.0);
}
