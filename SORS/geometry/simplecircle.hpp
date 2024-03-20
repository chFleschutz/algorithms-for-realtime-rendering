#ifndef SIMPLECIRCLE_HPP
#define SIMPLECIRCLE_HPP

#include "geometry/igeometryimplementation.hpp"

//! Eine einfache Klasse zum Zeichnen eines Kreises um den Ursprung in der xy-Ebene
//! Zeigt auch die Verwendung von GL_POINTS und GL_LINES
class SimpleCircle: public IGeometryImplementation
{
public:
    SimpleCircle(float pRadius = 5.0f, float pNrOfPoints = 16.0f, bool pDrawPoints = true)
        : mRadius(pRadius)
        , mNrOfPoints(pNrOfPoints)
        , mDrawPoints(pDrawPoints)
    {}

protected:
    virtual void fillVertices(QVector<QVector4D> &vertices) override;
    float mRadius;
    float mNrOfPoints;
    bool mDrawPoints;
    virtual void render() override;
};

#endif // SIMPLECIRCLE_HPP
