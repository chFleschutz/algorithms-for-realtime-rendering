layout (points) in;
layout (triangle_strip) out;
layout (max_vertices=4) out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform float pSizeX=2.0;
uniform float pSizeY=2.0;

vec3 worldvec = vec3(0.0,0.0,-1.0);

out vec2 texCoords;

void main()
{
    //Calculate View-Direction of Camera
    //which is the transformation of the z-axis
    //or the 3rd column of the MV-Matrix (but beware GLSL columnwise mats
    vec3 cameraRotation = normalize(viewMatrix[2].xyz);//

    //Vertices are in WC
    for(int i=0; i<gl_in.length(); ++i) {

        // TODO: Hier Code aus Aufgabe 2 (Billboards) für lCenterOfBillboard und lPlaneVec einfügen
        vec3 lCenterOfBillboard, lPlaneVec;

        // first calculate the interiorangle with acos
        // TODO: interiorangle berechnen (Winkel zwischen cameraRotation-Vektor und world-Vektor berechnen)

        // change rad to degree
        interiorangle = degrees(interiorangle);
        // acos only gives back numbers between 0 and 180
        // so we have to adjust that for negative cameraRotations
        //can be done with atan2 or like here a little more obvious explicitly calculating ...
        if(interiorangle >=22.5 && interiorangle< 157.5 && cameraRotation.x<0)
        {
            interiorangle+=180;
        }
        interiorangle = mod(interiorangle,360);
        // Array to pick the right part of the texture
        // Top left to Bottom down
        //OpenGL prior to 4.3 does not allow multidimensional C-Style arrays,
        //therefore unusual initialization
        vec2 imposterIndexArray[9*4]= vec2[9*4](    // 9 Zeilen, 4 Spalten
                                                    // 2er Vektoren für richtige Texturzuordnung
                                                    // links unten, links oben, rechts unten, rechts oben
                                        vec2(0.0,0.66),vec2(0.0,1.0),vec2(0.33,0.66),vec2(0.33,1.0),
                                        vec2(0.33,0.66),vec2(0.33,1.0),vec2(0.66,0.66),vec2(0.66,1.0),
                                        vec2(0.66,0.66),vec2(0.66,1.0),vec2(1.0,0.66),vec2(1.0,1.0),
                                        vec2(0.0,0.33),vec2(0.0,0.66),vec2(0.33,0.33),vec2(0.33,0.66),
                                        vec2(0.33,0.33),vec2(0.33,0.66),vec2(0.66,0.33),vec2(0.66,0.66),
                                        vec2(0.33,0.0),vec2(0.33,0.33),vec2(0.66,0.0),vec2(0.66,0.33),
                                        vec2(0.0,0.0),vec2(0.0,0.33),vec2(0.33,0.0),vec2(0.33,0.33),
                                        vec2(0.66,0.33),vec2(0.66,0.66),vec2(1.0,0.33),vec2(1.0,0.66),
                                        vec2(0.66,0.0),vec2(0.66,0.33),vec2(1.0,0.0),vec2(1.0,0.33)
                                       );

        // set index according to the angle
        int imposterIndex;
        // if(interiorangle > something && interiorangle < something else
        // {imposterIndex = index in the array}
        // explicit if statements could be done more elegantly by calculus
        if((interiorangle >= 337.5 && interiorangle <= 360)|| (interiorangle < 22.5 && interiorangle >= 0))
        {imposterIndex = 0;}
        else if(interiorangle >= 22.5 && interiorangle < 67.5)
        {imposterIndex = 1;}
        else if...
        // TODO: else if-Schleifen vervollständigen

        // Billboard Impostor Vertex links unten berechnen, texCoords zuweisen und ausgeben:
        gl_Position = projectionMatrix*vec4(lCenterOfBillboard-pSizeX/2.0*lPlaneVec-pSizeY/2.0*vec3(0,1,0),1);
        // TODO: texCoords zuweisen (mit passendem imposterIndex auf das Array der Texturkoordinaten zugreifen)
        EmitVertex();

        // TODO: gl_Position fuer die restlichen drei Eckpunkte berechnen
        // TODO: texCoords zuweisen (mit passendem imposterIndex auf das Array der Texturkoordinaten zugreifen)
        // Eckpunkt ausgeben

        EndPrimitive();
    }
}
