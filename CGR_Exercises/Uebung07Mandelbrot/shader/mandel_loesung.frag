out vec4 fColor;

uniform bool hastextureMap=true;
uniform sampler2D textureMap;

uniform float MaxIterations=1000;
uniform float Zoom=1.0;
uniform vec2  Center=vec2(0.0,0.0);
uniform vec3  InnerColor=vec3(1.0,0.0,0.0);
uniform vec3  OuterColor1=vec3(0.0,1.0,0.0);
uniform vec3  OuterColor2=vec3(0.0,0.0,1.0);
uniform bool  RenderWithShader = true;

in vec2 texCoords;

void main()
{
    if (RenderWithShader)
    {
        vec2 Position=texCoords-vec2(0.5,0.5);

        float   real  = Position.x * Zoom + Center.x;
        float   imag  = Position.y * Zoom + Center.y;
        float   Creal = real;
        float   Cimag = imag;

        float r2 = 0.0;
        float iter;

        for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
        {
            float tempreal = real;

            real = (tempreal * tempreal) - (imag * imag) + Creal;
            imag = 2.0 * tempreal * imag + Cimag;
            r2   = (real * real) + (imag * imag);
        }

        vec3 color;

        if (r2 < 4.0)
            color = InnerColor;
        else
            color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));

        fColor = vec4(color, 1.0);
    }
    else
    {
        vec4 textureFrag = texture(textureMap, texCoords);
        fColor = vec4(textureFrag.rgb, textureFrag.a);
    }
}
