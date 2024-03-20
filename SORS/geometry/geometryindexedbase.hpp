#ifndef GEOMETRYINDEXEDBASE_H
#define GEOMETRYINDEXEDBASE_H

#include "geometry/geometrybase.hpp"
#include "geometry/igeometryindexedimplementation.hpp"

//! Basisklasse für alle Geometrien, die indexed gespeichert sind. Indexed bedeutet, dass nicht bei jedem
//! OpenGL-DrawCall die Vertices explizit (also durch ihre Geometriedaten) übergeben werden. Statt dessen
//! gibt es einen Vertexbuffer mit allen Vertices und ihren Geometriedaten und einen Indexbuffer,
//! der dann die Triangles über die Indices der Vertices des Triangle enthält.
class GeometryIndexedBase: public GeometryBase
{
public:
    GeometryIndexedBase();
    GeometryIndexedBase(std::shared_ptr<IGeometryIndexedImplementation> pImplementation);
    virtual void prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao) override;
    virtual void prepareVertexBuffers() override;
    virtual void destroyVertexBuffers() override;

    //! Gibt die Vertices als Dreiecksliste zurück. Also nicht geindexed.
    virtual QVector<QVector4D> getTriangleList() override;

private:
    QVector<GLuint> mIndices;
    QOpenGLBuffer mIndexBuf;
};

#endif // GEOMETRYINDEXEDBASE_H
