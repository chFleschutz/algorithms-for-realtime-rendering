#ifndef SIMPLECIRCLEFILLED_HPP
#define SIMPLECIRCLEFILLED_HPP

#include "opengl.hpp"
#include "simplecircle.hpp"

class SimpleCircleFilled: public SimpleCircle {
public:
    SimpleCircleFilled(float pRadius = 5.0f, float pNrOfPoints = 16.0f): SimpleCircle(pRadius, pNrOfPoints) {}

    void fillVertices(QVector<QVector4D> &vertices) override {
        SimpleCircle::fillVertices(vertices);
        vertices.append(vertices.first());
        vertices.prepend(QVector4D(.0f,.0f,.0f,1.f));
    }

    virtual void render() override {
        glFunctions->glDrawArrays(GL_TRIANGLE_FAN, 0, mNrOfPoints+2);
    }
};

#endif // SIMPLECIRCLEFILLED_HPP
