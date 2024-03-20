in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

out vec3 texCoords;

void main()
{
    vec4 positionWithoutTranslation;
    mat4 pMatrixWoT = projectionMatrix, mvMatrixWoT = modelViewMatrix;
    pMatrixWoT[3] = vec4(0.,0.,0.,1.);
    mvMatrixWoT[3] = vec4(0.,0.,0.,1.);
    positionWithoutTranslation = pMatrixWoT * mvMatrixWoT * vertexPosition;
    gl_Position = positionWithoutTranslation;

    //Die Koordinatenachsen bei OpenGL Cubemaps stimmen nicht mit den typischen Cubemaps (Renderman, DirectX, ...) überein
    //siehe hierzu GLSpec 4.5 Core, Seite 240

    texCoords = vec3(vertexPosition.x, -vertexPosition.yz);
}
