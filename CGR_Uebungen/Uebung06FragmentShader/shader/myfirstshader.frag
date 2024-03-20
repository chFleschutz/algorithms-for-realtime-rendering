//Einkommentieren, um Farbe an Fragment Shader zu übergeben
//in vec3 color;
//Fragment Shader können nur einen Ausgabewert haben, der automatisch die Farbe des Fragments ist
out vec4 fragColor;

void main()
{
//Einkommentieren, um übergeben Farbe auszugeben
//    fragColor = vec4(color, 1.0);

    //"Zerlegt" Fenster in Abschnitte, die je 25 Pixel groß sind
    vec2 pos = mod(gl_FragCoord.xy, vec2(50.0)) - vec2(25.0);
    //Abstand zum Mittelpunkt des Abschnitts
    float dist_squared = dot(pos, pos);

    //Wenn im Kreis mit Radius 20, dann farbig zeichnen, sonst grau
     fragColor = (dist_squared < 400.0)
         ? vec4(.90, .90, .10, 1.0)
         : vec4(.20, .20, .20, 1.0);
     //Dasselbe lässt sich (performanter) dadurch erreichen - warum?
     fragColor = mix(vec4(.90, .90, .10, 1.0), vec4(.20, .20, .20, 1.0),
                    step(400.0, dist_squared));
}
