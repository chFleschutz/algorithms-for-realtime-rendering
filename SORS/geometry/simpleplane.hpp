#ifndef SIMPLEPLANE_HPP
#define SIMPLEPLANE_HPP

#include "geometry/igeometryimplementation.hpp"

//! Einfache Fläche in parametrisierbarer Ausdehnung und Auflösung (mDivs).
class SimplePlane: public IGeometryImplementation
{
public:
    SimplePlane(float pSize): mXSize(pSize), mYSize(pSize), mDivs(1) {}
    SimplePlane(float pSizeX, float pSizeY): mXSize(pSizeX), mYSize(pSizeY), mDivs(1) {}
    SimplePlane(float pSizeX, float pSizeY, unsigned int pDivs): mXSize(pSizeX), mYSize(pSizeY), mDivs(pDivs) {}
    virtual void render() override;

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

protected:
    float mXSize;
    float mYSize;
    unsigned int mDivs;
};

#endif // SIMPLEPLANE_HPP
