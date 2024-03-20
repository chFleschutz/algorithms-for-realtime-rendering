#ifndef COLOR_HPP
#define COLOR_HPP

#include "irenderableproperty.hpp"

//! Einfache Property, um eine Farbe an den Shader zu übergeben. Beispielcode:
//! @code
//! auto lColor = std::make_shared<Color>(pR, pG, pB, 1.f);
//! ECS.get<Renderable>(myEntity).addProperty(lColor);
//! @endcode
class Color: public IRenderableProperty
{
public:
    Color(){}
    Color(float pRed, float pGreen, float pBlue, float pAlpha): mColor(pRed, pGreen, pBlue, pAlpha) {}
    Color(QVector4D pColor): mColor(pColor) {}

    void setColor(const QVector4D &pColor) {mColor = pColor;}
    const QVector4D getColor() {return mColor;}
    void preRender(Shader &pShader) override {pShader.shaderProgram()->setUniformValue("color", mColor);}

protected:
    QVector4D mColor{1.0f, 0.0f, 0.0f, 1.0f};
};

#endif // COLOR_HPP
