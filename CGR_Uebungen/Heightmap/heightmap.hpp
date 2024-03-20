#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <QImage>
#include "geometry/igeometryimplementation.hpp"

class Heightmap: public IGeometryImplementation
{
public:
    Heightmap();
    void loadImage(QString pPath);
//    virtual void keyboard(int key, int modifier) override;
    void setPoints(bool p) {mPoints = p;}

    //!Methoden aus @see IGeometryImplementation überschreiben
    virtual void render() override;

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillColors(QVector<QVector4D>& colors) override;
    void keyboard();
private:
    QImage image;
    QString image_pfade[5];
    float myScale;
    bool mPoints;
    float clampColor(uchar d)	{return (d/255.0);}
    double scaleTo(int toscale);
    QVector3D newell(QList<QVector3D> pInputVertices);
};

#endif // HEIGHTMAP_HPP
