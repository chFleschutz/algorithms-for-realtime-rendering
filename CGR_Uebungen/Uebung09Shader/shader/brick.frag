in vec2 Pos_mc;			//Pos in model coord.
out vec4 fragColor;

uniform vec2 BrickStepSize = vec2(1.0f,0.5f);
uniform vec2 BrickRatio = vec2(0.9f,0.8f);

void main()
{
    vec2 pos = Pos_mc / BrickStepSize, useBrick;
    vec3 BrickColor = vec3(1.0,0.0,0.0), MortarColor = vec3(0.5,0.5,0.5);
    vec3 color;

    // Wechsel gerade/ungerade
    if ( fract(pos.y * 0.5) > 0.5 )	//Verwendung eingebauter Fkt. für Performance!
            pos.x += 0.5;

    pos = fract( pos );

    useBrick = step(pos, BrickRatio);
    color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);

    fragColor = vec4(color, 1.0);
}
