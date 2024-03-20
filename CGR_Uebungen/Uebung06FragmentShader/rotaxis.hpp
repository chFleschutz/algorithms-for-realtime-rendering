#ifndef ROTAXIS_HPP
#define ROTAXIS_HPP

#include <QVector3D>
#include "rendering/irenderableproperty.hpp"
#include "input/inputregistry.hpp"

class RotAxisProperty: public IRenderableProperty
{
public:
    RotAxisProperty(){}
    void preRender(Shader &pShader) override {
        auto keyIn = InputRegistry::getInstance().getKeyboardInput();

        if (keyIn->isKeyPressed('u'))
            mRotAxis.setX(std::max(0.1f, mRotAxis.x() / 1.1F));
        else if (keyIn->isKeyPressed('U'))
            mRotAxis.setX(std::min(1.0f, mRotAxis.x() * 1.1F));
        else if (keyIn->isKeyPressed('i'))
            mRotAxis.setY(std::max(0.1f, mRotAxis.y() / 1.1F));
        else if (keyIn->isKeyPressed('I'))
            mRotAxis.setY(std::min(1.0f, mRotAxis.y() * 1.1F));
        else if (keyIn->isKeyPressed('o'))
            mRotAxis.setZ(std::max(0.1f, mRotAxis.z() / 1.1F));
        else if (keyIn->isKeyPressed('O'))
            mRotAxis.setZ(std::min(1.0f, mRotAxis.z() * 1.1F));

        pShader.shaderProgram()->setUniformValue("rotAxis", mRotAxis);
    }

protected:
    QVector3D mRotAxis{.5f,.5f,.5f};
};

#endif // ROTAXIS_HPP
