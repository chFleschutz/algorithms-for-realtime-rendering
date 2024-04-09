in vec4 vertexPosition;
in vec2 textureCoords;

out vec2 texCoords;

void main()
{
    // TODO: Aufgabe 1.2 - Model-View-Matrix und Projektionsmatrix anpassen
    mat4 mvMatrix =
            mat4(vec4(0,0,0,0),
                vec4(0,0,0,0),
                vec4(0,0,0,0),
                vec4(0,0,0,0));

    mat4 projMatrix =
            mat4(vec4(0,0,0,0),
                vec4(0,0,0,0),
                vec4(0,0,0,0),
                vec4(0,0,0,0));

    texCoords=textureCoords;
    gl_Position = vertexPosition;
    //gl_Position = projMatrix * mvMatrix * vertexPosition;
}
