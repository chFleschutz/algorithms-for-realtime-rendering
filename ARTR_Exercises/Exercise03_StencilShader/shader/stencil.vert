in vec4 vertexPosition;
in vec2 textureCoords;

out vec2 texCoords;

void main()
{
    // TODO: Aufgabe 1.2 - Model-View-Matrix und Projektionsmatrix anpassen
    mat4 mvMatrix = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, -1.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    float n = 0.01;
    float f = 100.0;
    float r = 1.0;
    float t = 1.0;
    mat4 projMatrix = mat4(
		vec4(n/r, 0.0, 0.0, 0.0),
		vec4(0.0, n/t, 0.0, 0.0),
		vec4(0.0, 0.0, -(f+n)/(f-n), -2.0*f*n/(f-n)),
		vec4(0.0, 0.0, -1.0, 0.0)
	);

    texCoords = textureCoords;
//    gl_Position = vertexPosition;
	gl_Position = mvMatrix * vertexPosition;
//    gl_Position = projMatrix * mvMatrix * vertexPosition;
}
