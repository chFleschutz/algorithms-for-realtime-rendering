#ifndef SIMPLECUBE_H
#define SIMPLECUBE_H

#include "opengl.hpp"
#include "geometry/igeometryimplementation.hpp"

//! Geometrie eines einfachen Würfels mit Normalen und Texturkoordinaten
class SimpleCube : public IGeometryImplementation
{
protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

private:
    GLfloat mWidth, mHeight, mDepth;

public:
    SimpleCube();
    SimpleCube(GLfloat pSize);
    SimpleCube(GLfloat pWidth, GLfloat pHeight, GLfloat pDepth);
    float getW(){return mWidth;}
    virtual void render() override;
    ~SimpleCube();
};

#endif
