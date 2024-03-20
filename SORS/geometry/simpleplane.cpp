#include "win_oglbase/openglwidget.hpp"
#include <QList>
#include <QVector4D>

#include "simpleplane.hpp"

void SimplePlane::fillVertices(QVector<QVector4D>& vertices)
{
    vertices.clear();
    for (unsigned int y = 0; y < mDivs; y++)
    {
        for (unsigned int x = 0; x <= mDivs; x++)
        {
            vertices.append(QVector4D(x / (float)mDivs * mXSize - mXSize / 2, (y + 1) / (float)mDivs * mYSize - mYSize / 2, .0, 1.0f));
            vertices.append(QVector4D(x / (float)mDivs * mXSize - mXSize / 2, y / (float)mDivs * mYSize - mYSize / 2, .0, 1.0f));
        }
    }
}

void SimplePlane::fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& /*vertices*/)
{
    normals.clear();
    for (unsigned int y = 0; y < mDivs; y++)
    {
        for (unsigned int x = 0; x <= mDivs; x++)
        {
            normals.append(QVector3D(.0, .0, 1.));
            normals.append(QVector3D(.0, .0, 1.));
        }
    }
}

void SimplePlane::fillTexCoords(QVector<QVector2D>& texCoords)
{
    for (unsigned int y = 0; y < mDivs; y++)
    {
        for (unsigned int x = 0; x <= mDivs; x++)
        {
            texCoords.append(QVector2D(x / (float)mDivs, (y + 1) / (float)mDivs));
            texCoords.append(QVector2D(x / (float)mDivs, y / (float)mDivs));
        }
    }
}

void SimplePlane::render()
{
    for (unsigned int y = 0; y < mDivs; y++)
    {
        glFunctions->glDrawArrays(GL_TRIANGLE_STRIP, y * (2 * mDivs + 2), 2 * mDivs + 2);
    }
}
