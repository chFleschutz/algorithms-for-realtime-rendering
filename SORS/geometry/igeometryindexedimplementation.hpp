#ifndef IGEOMETRYINDEXEDIMPLEMENTATION_H
#define IGEOMETRYINDEXEDIMPLEMENTATION_H

#include "opengl.hpp"
#include "geometry/igeometryimplementation.hpp"

//! Spezialisierung der Klasse IGeometryImplementation, um indexed geometries zu unterstützen.
class IGeometryIndexedImplementation: public IGeometryImplementation
{
public:
    IGeometryIndexedImplementation() {}
    //! Überschrieben um die Indices beim Initialisieren der Geometrie zu erstellen.
    virtual void fillIndices(QVector<GLuint>& indices) {}

    void render() override
    {
        glFunctions->glDrawElements(GL_TRIANGLES, mNrOfIndices, GL_UNSIGNED_INT, 0);
    }

protected:
    unsigned int mNrOfIndices;

};

#endif // IGEOMETRYINDEXEDIMPLEMENTATION_H
