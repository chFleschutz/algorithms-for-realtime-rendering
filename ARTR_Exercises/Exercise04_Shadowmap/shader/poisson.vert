layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

//output data is interpolated for each fragment
out vec4 outputColor;
out vec4 ShadowCoord;

uniform mat4 projectionMatrix; // projection
uniform mat4 MVPLight;
uniform mat4 modelViewLight;
uniform mat3 normalMatrixLight;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform bool buildingShadowmap;
uniform vec4 color;

void main(){

    if (buildingShadowmap){
            gl_Position = projectionMatrix * (viewMatrix * modelMatrix) * vertexPosition;
    }else{
        gl_Position = projectionMatrix * (viewMatrix * modelMatrix) * vertexPosition;

        ShadowCoord = MVPLight * modelMatrix * vertexPosition;

        outputColor = color;
    }
}
