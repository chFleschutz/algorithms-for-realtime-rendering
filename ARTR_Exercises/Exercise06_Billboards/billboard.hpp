#ifndef BILLBOARD_HPP
#define BILLBOARD_HPP

#include "geometry/geometrybase.hpp"
#include "ecs.hpp"
#include "geometry/igeometryimplementation.hpp"

class Billboard: public IGeometryImplementation
{
public:
    Billboard(QList<QVector4D> pPoints)
    {
        mPoints += pPoints; //We want a deep copy
    }

    virtual void render() override
    {
        glFunctions->glDrawArrays(GL_POINTS, 0, mPoints.length());
    }

    virtual void fillVertices(QVector<QVector4D> &vertices) override
    {
        vertices.clear();
        QList<QVector4D>::iterator i;
        for (i=mPoints.begin(); i!=mPoints.end(); i++)
        {
            vertices.append(*i);
        }
    }

protected:
    QList<QVector4D> mPoints;
};

#endif // BILLBOARD_HPP
