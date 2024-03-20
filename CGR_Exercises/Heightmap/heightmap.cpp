#include "heightmap.hpp"

#include "QRgb"
#include "geometry/geometrybase.hpp"
#include "heightmap.hpp"
#include "input/inputregistry.hpp"
#include "ecs.hpp"

void Heightmap::loadImage(QString pPath)
{
    auto lPath = QString(DEFAULT_IMAGES_DIR) + pPath;
    Q_ASSERT(image.load(lPath));
}

Heightmap::Heightmap(): myScale(5.0), mPoints(false)
{
    image_pfade[0] = "sam.bmp";
    image_pfade[1] = "logo.bmp";
    image_pfade[2] = "sw.bmp";
    image_pfade[3] = "terrain.bmp";
    image_pfade[4] = "swklein.bmp";
    loadImage(image_pfade[0]);
}

double Heightmap::scaleTo(int toscale)
{
    static int imageausdehnung=std::max(image.height(),image.width());

    return double(toscale)/imageausdehnung*myScale;
}

void Heightmap::fillColors(QVector<QVector4D>& colors)
{
    int i, j;

    colors.clear();
    for (i=0; i<image.height()-1; i++)
        for (j=0; j<image.width()-1; j++)
        {
            //Dreieck links oben
            QRgb farbe_ji = image.pixel(j, i);
            QRgb farbe_jiplus1 = image.pixel(j, i+1);
            QRgb farbe_jplus1i = image.pixel(j+1, i);
            QRgb farbe_jplus1iplus1 = image.pixel(j+1, i+1);
            colors.append(QVector4D(clampColor(qRed(farbe_ji)),clampColor(qGreen(farbe_ji)), clampColor(qBlue(farbe_ji)), 1.0));
            if (!mPoints)
                colors.append(QVector4D(clampColor(qRed(farbe_jplus1i)),clampColor(qGreen(farbe_jplus1i)), clampColor(qBlue(farbe_jplus1i)), 1.0));
            colors.append(QVector4D(clampColor(qRed(farbe_jiplus1)),clampColor(qGreen(farbe_jiplus1)), clampColor(qBlue(farbe_jiplus1)), 1.0));
            //Dreieck rechts unten
            if (!mPoints)
                colors.append(QVector4D(clampColor(qRed(farbe_jplus1i)),clampColor(qGreen(farbe_jplus1i)), clampColor(qBlue(farbe_jplus1i)), 1.0));
            colors.append(QVector4D(clampColor(qRed(farbe_jplus1iplus1)),clampColor(qGreen(farbe_jplus1iplus1)), clampColor(qBlue(farbe_jplus1iplus1)), 1.0));
            colors.append(QVector4D(clampColor(qRed(farbe_jiplus1)),clampColor(qGreen(farbe_jiplus1)), clampColor(qBlue(farbe_jiplus1)), 1.0));
        }
}

void Heightmap::fillVertices(QVector<QVector4D> & vertices)
{
    int i, j;

    vertices.clear();
    for (i=0; i<image.height()-1; i++)
        for (j=0; j<image.width()-1; j++)
        {
            //Dreieck links oben
            QRgb farbe_ji = image.pixel(j, i);
            QRgb farbe_jiplus1 = image.pixel(j, i+1);
            QRgb farbe_jplus1i = image.pixel(j+1, i);
            QRgb farbe_jplus1iplus1 = image.pixel(j+1, i+1);
            vertices.append(QVector4D(scaleTo(i)-scaleTo(image.height())/2, scaleTo(qGray(farbe_ji))/10.0, scaleTo(j),1.0));
            if (!mPoints)
                vertices.append(QVector4D(scaleTo(i)-scaleTo(image.height())/2, scaleTo(qGray(farbe_jplus1i))/10.0, scaleTo(j+1),1.0));
            vertices.append(QVector4D(scaleTo(i+1)-scaleTo(image.height())/2, scaleTo(qGray(farbe_jiplus1))/10.0, scaleTo(j),1.0));
            //Dreieck rechts unten
            if (!mPoints)
                vertices.append(QVector4D(scaleTo(i)-scaleTo(image.height())/2, scaleTo(qGray(farbe_jplus1i))/10.0, scaleTo(j+1),1.0));
            vertices.append(QVector4D(scaleTo(i+1)-scaleTo(image.height())/2, scaleTo(qGray(farbe_jplus1iplus1))/10.0, scaleTo(j+1),1.0));
            vertices.append(QVector4D(scaleTo(i+1)-scaleTo(image.height())/2, scaleTo(qGray(farbe_jiplus1))/10.0, scaleTo(j),1.0));
        }
}

void Heightmap::keyboard()
{
    auto keyIn = InputRegistry::getInstance().getKeyboardInput();
    mNeedsInit = true;

    if (keyIn->isKeyPressed('g'))
        myScale /= 1.1F;
    else if (keyIn->isKeyPressed('G'))
        myScale *= 1.1F;
    else if (keyIn->isKeyPressed('p'))
        mPoints = true;
    else if (keyIn->isKeyPressed('l'))
        mPoints = false;
    else if (keyIn->isKeyPressed('1'))
        loadImage(image_pfade[0]);
    else if (keyIn->isKeyPressed('2'))
        loadImage(image_pfade[1]);
    else if (keyIn->isKeyPressed('3'))
        loadImage(image_pfade[2]);
    else if (keyIn->isKeyPressed('4'))
        loadImage(image_pfade[3]);
    else if (keyIn->isKeyPressed('t'))
        mPoints=!mPoints;
    else
        mNeedsInit = false;
}

void Heightmap::render()
{
    keyboard();
    if (mPoints)
    {
        glFunctions->glPointSize(5.0);
        glFunctions->glDrawArrays(GL_POINTS,0, 4*(image.height()-1)*(image.width()-1));
    }
    else
        glFunctions->glDrawArrays(GL_TRIANGLES,0, 6*(image.height()-1)*(image.width()-1));
}
