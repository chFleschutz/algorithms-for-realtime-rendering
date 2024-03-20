#include "opengl.hpp"
#include "simplecircle.hpp"

void SimpleCircle::fillVertices(QVector<QVector4D> &vertices)
{
    //Bei Linien muss der letzte Vertex immer nochmal als Anfang der Linie übergeben werden
    QVector4D mLastPoint(mRadius, 0.0f, 0.0f, 1.0f);
    vertices.clear();

    for (auto t = 0.0f; t < 2.0f * M_PI; t += 2.0f * M_PI / mNrOfPoints) {
        if (!mDrawPoints)
            vertices.append(mLastPoint);
        mLastPoint = QVector4D(mRadius * cos(t), mRadius * sin(t), 0.0f, 1.0f);
        vertices.append(mLastPoint);
    }
    //Bei Linien Ring schließen
    if (!mDrawPoints) {
        vertices.append(mLastPoint);
        vertices.append(QVector4D(mRadius, 0.0f, 0.0f, 1.0f));
    }
}

void SimpleCircle::render()
{
    glFunctions->glPointSize(10.0f);
    glFunctions->glLineWidth(5.0f);

    if (mDrawPoints)
        glFunctions->glDrawArrays(GL_POINTS, 0, mNrOfPoints);
    else
        glFunctions->glDrawArrays(GL_LINES, 0, 2.0f * mNrOfPoints + 2.0f);
}
