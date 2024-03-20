#ifndef SIMPLESPHERE_H
#define SIMPLESPHERE_H

#include "geometry/igeometryindexedimplementation.hpp"

//! Einfache Klasse zum Zeichnen einer tesselierten Kugel mit indexed geometry
class SimpleSphere: public IGeometryIndexedImplementation
{
public:
    SimpleSphere(float size = 1.0, int subdivisionHorizontal = 25, int subdivisionVertical = 25);
    virtual ~SimpleSphere();

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;
    virtual void fillIndices(QVector<GLuint>& indices) override;

private:
    int mSubdivisionHorizontal;
    int mSubdivisionVertical;
    float mSize;
};

#endif // SIMPLESPHERE_H
