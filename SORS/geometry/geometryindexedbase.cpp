#include "manager.hpp"

#include "geometryindexedbase.hpp"

GeometryIndexedBase::GeometryIndexedBase(): mIndexBuf(QOpenGLBuffer::IndexBuffer) {
    mIndices.clear();
    GeometryBase::addRenderableClass<GeometryIndexedBase>();
}

GeometryIndexedBase::GeometryIndexedBase(std::shared_ptr<IGeometryIndexedImplementation> pImplementation):
    GeometryBase(pImplementation), mIndexBuf(QOpenGLBuffer::IndexBuffer)
{
}

void GeometryIndexedBase::prepareVertexBuffers()
{
    auto lImplementation = std::static_pointer_cast<IGeometryIndexedImplementation>(mImplementation);

    lImplementation->fillIndices(mIndices);

    GeometryBase::prepareVertexBuffers();

    if (!mIndices.isEmpty())
    {
        initBuffer(mIndexBuf, mIndices);
    }
}

void GeometryIndexedBase::prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao)
{
    GeometryBase::prepareVertexArrayObject(shader, vao);
    if (!mIndices.isEmpty())
    {
        mIndexBuf.bind();
    }
}

void GeometryIndexedBase::destroyVertexBuffers()
{
    GeometryBase::destroyVertexBuffers();
    mIndexBuf.destroy();
    mIndices.clear();
}

QVector<QVector4D> GeometryIndexedBase::getTriangleList()
{
    QVector<QVector4D> deindexedVertices;
    deindexedVertices.reserve(mIndices.size());
    for (int i = 0; i < mIndices.size(); ++i)
    {
        deindexedVertices.append(mVertices[mIndices[i]]);
    }

    return deindexedVertices;
}
