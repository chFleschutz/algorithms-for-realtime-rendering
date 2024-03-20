#ifndef IGEOMETRYIMPLEMENTATION_H
#define IGEOMETRYIMPLEMENTATION_H

#include <QVector4D>

#include "thirdparty/entt/single_include/entt/entt.hpp"

//! Alle Implementierungen gehören zu einer der von GeometryBase abgeleiteten eigentlichen Geometrien.
//! Bei der Initialisierung der Geometrien wird die Implementierung benutzt, um die Geometrie zu "befüllen".
//! Folgt lose dem dependency injection Pattern.
class IGeometryImplementation
{
public:
    IGeometryImplementation() {}
    //! Überschreiben um die Vertices beim Initialisieren der Geometrie zu erstellen.
    virtual void fillVertices(QVector<QVector4D>& vertices) {}

    //! Überschreiben um die Normalen beim Initialisieren der Geometrie zu erstellen.
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) {}

    //! Überschreiben um die Textur-Koordinaten beim Initialisieren der Geometrie zu erstellen.
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) {}

    //! Überschreiben um die Vertex-Farben beim Initialisieren der Geometrie zu erstellen.
    virtual void fillColors(QVector<QVector4D>& colors) {}

    //! Muss von Implementierungen überschrieben werden, wenn init während des Renderloops gerufen werden muss
    virtual bool needsInit() {return mNeedsInit;}
    //! Wird vor dem Rendern gerufen, wenn needsInit true zurückgibt
    //!  Alle Inits der Geometrien im preRender gesammelt durchgeführt
    virtual void init() {}

    //! Objekt zeichnen
    virtual void render() = 0;
    void setNeedsInit(bool pNeedsInit);

protected:
    //! Sagt aus, ob die Geometrie neu initialisiert und die GeometryBase neu
    //! befüllt werden muss
    //! Nicht ganz "sauber" für ein Interface, aber sinnvoll
    bool mNeedsInit{true};
};

inline void IGeometryImplementation::setNeedsInit(bool pNeedsInit)
{
    mNeedsInit = pNeedsInit;
}

#endif // IGEOMETRYIMPLEMENTATION_H
