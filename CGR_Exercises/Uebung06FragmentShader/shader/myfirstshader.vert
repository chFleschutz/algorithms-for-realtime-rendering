//Allgemeine Uniforms vom Szenengraphen
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform float Time; //Schauen Sie sich hierzu ShaderTimed an
uniform vec3 rotAxis = vec3(1.0,.0,.0); //Kommt aus RotAxisProperty

//Einkommentieren, um Farbe an Fragment Shader zu übergeben
//out vec3 color;

layout (location = 0) in vec4 vertexPosition;

mat4 rotXAchse(float pAngle)
{
//Immer daran denken, dass GLSL Column major order macht. D.h. der erste Vektor ergibt die erste Spalte
return mat4(1, 0,	0, 0,
            0, cos(pAngle), sin(pAngle), 0,
            0, -sin(pAngle),  cos(pAngle), 0,
            0, 0, 0, 1);
}

mat4 rotYAchse(float pAngle)
{
return mat4(cos(pAngle), 0, -sin(pAngle), 0,
            0, 1, 0, 0,
            sin(pAngle),  0, cos(pAngle), 0,
            0, 0, 0, 1);
}

mat4 rotZAchse(float pAngle)
{
return mat4(cos(pAngle), sin(pAngle), 0, 0,
            -sin(pAngle),  cos(pAngle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
}

float calculateYAngle(vec3 pAxis)
{
    vec3 lAxisinXZ = vec3(pAxis.x, 0, pAxis.z);
    int richtung = int(sign(pAxis.z));
    float lInnerProduct = dot(lAxisinXZ, vec3(1,0,0))/length(lAxisinXZ);
    return (richtung * acos(lInnerProduct));
}

float calculateZAngle(vec3 pAxis)
{
    vec3 lAxisinXY = vec3(pAxis.x, pAxis.y, 0);
    int richtung = int(-sign(pAxis.x));
    float lInnerProduct = dot(lAxisinXY, vec3(1,0,0))/length(lAxisinXY);
    return (richtung * acos(lInnerProduct));
}

void main()
{
    vec3 lRotAxis = normalize(rotAxis);

    //Folgende Unterscheidung löst das Problem, dass bei den Projektionen oben der Nullvektor entstehen kann
    //Das passiert z.B., wenn die y-Achse als Drehachse angegeben wird
    //Man vermeidet das, indem man in solchen Fällen eine weitere Drehung vorab macht (und hinterher natürlich wieder rückgängig)
    //An sich vermeidet man if-Statements in Shadern. In diesem Fall aber vertretbar, da auf der Basis von uniform Werten verzweigt wird
    //In solchen Fällen wird pro Rendering immer derselbe Zweig genommen, so dass der GLSL-Compiler dies berücksichtigen kann und wird
    //Eine alternative Formulierung wäre unter verwendung der step-Funktion moeglich nach der Logik
    //(a >= b) ? c : 0 kann ersetzt werden durch step(a, b) * c
    mat4 getOutOfYLock = mat4(1.0); //Einheitsmatrix
    if (length(lRotAxis.y) > length(lRotAxis.x))
    {
        getOutOfYLock = rotZAchse(90);
        lRotAxis = vec3(getOutOfYLock * vec4(lRotAxis, 0.0));
    }

    mat4 lRotMatrix = rotYAchse(calculateYAngle(lRotAxis))*rotZAchse(calculateZAngle(lRotAxis));
    gl_Position = projectionMatrix * modelViewMatrix * inverse(getOutOfYLock) * inverse(lRotMatrix) * rotXAchse(Time*45)* lRotMatrix * getOutOfYLock * vertexPosition;
    //Farbe an Fragment Shader übergeben
    //    color = gl_Position.xyz;
}
