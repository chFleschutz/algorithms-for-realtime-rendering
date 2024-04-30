layout (points) in;
layout (triangle_strip) out;
layout (max_vertices=4) out;

uniform mat4 projectionMatrix;

uniform float pSizeX=2.5;
uniform float pSizeY=3.5;

//Variable für Billboards erstellen
out vec2 texCoords;

void main()
{
    //Vertices are in WC
    for(int i=0; i<gl_in.length(); ++i) {
        vec3 lCenterOfBillboard, lPlaneVec;

        // TODO: lCenterOfBillboard = Vektor zum Billboard-Mittelpunkt speichern (vec3)

        // TODO: lPlaneVec = Vektor für Blickrichtung der Kamera definieren (Kreuzprodukt des normalisierten lCenterOfBillboard-Vektors und des up-Vektors)

        // Vertex links unten definieren:
        gl_Position = projectionMatrix*vec4(lCenterOfBillboard
                            -pSizeX/2.0*lPlaneVec       //an den linken rand des quads
                            -pSizeY/2.0*vec3(0,1,0),1); //unten links 
        texCoords = vec2(0,0);
        EmitVertex();

        // TODO: gl_Position für die restlichen drei Eckpunkte berechnen
        // Passende TexturKoordinaten zuweisen
        // Eckpunkt ausgeben

        EndPrimitive();
    }
}
