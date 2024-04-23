layout (location = 0) in vec4 vertexPosition;
in vec4 vertexColor;

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
uniform bool hasColors = false;
out vec4 outputColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

#define COLOR_STEP 18

void main()
{
    //Vertexfarbe von der Geometrie gesetzt
    if (hasColors)
    {
        outputColor = vertexColor;
    }
    //sonst allgemeine Farbe von Uniform setzen
    else
    {
         outputColor = color;
    }

    // Calculate the vertex position
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
}
