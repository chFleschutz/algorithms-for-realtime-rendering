#ifndef LIGHT_HPP
#define LIGHT_HPP
#include "irenderableproperty.hpp"

//! Implementierung einer Punktlichtquelle, die dann im Shader zur Verfügung steht. Die Klasse
//! hält die wesentlichen Materialeigenschaften der Lichtquelle sowie Position und Richtung
//! und übergibt diese dann in preRender an den jeweils aktiven Shader.´
class PointLight: public IRenderableProperty
{
    void preRender(Shader &pShader) override {
        pShader.shaderProgram()->setUniformValue("light.ambient", mAmbient);
        pShader.shaderProgram()->setUniformValue("light.diffuse", mDiffuse);
        pShader.shaderProgram()->setUniformValue("light.specular", mSpecular);
        pShader.shaderProgram()->setUniformValue("light.viewPosition", mViewPosition);
        pShader.shaderProgram()->setUniformValue("light.attenuations", mAttenuations);
    }


public:
    QVector3D getAmbient() const;
    void setAmbient(const QVector3D &pAmbient);
    QVector3D getDiffuse() const;
    void setDiffuse(const QVector3D &pDiffuse);
    QVector3D getSpecular() const;
    void setSpecular(const QVector3D &pSpecular);
    QVector3D getViewPosition() const;
    void setViewPosition(const QVector3D &pViewPosition);
    QVector2D getAttenuations() const;
    void setAttenuations(const QVector2D &pAttenuations);

protected:
    QVector3D mAmbient{0.3f,0.3f,0.3f};
    QVector3D mDiffuse{1.f,1.f,1.f};
    QVector3D mSpecular{1.f,1.f,1.f};
    QVector3D mViewPosition{0.f,0.f,0.f};
    QVector2D mAttenuations{0.01f,0.001f};
};

inline QVector3D PointLight::getAmbient() const
{
    return mAmbient;
}

inline void PointLight::setAmbient(const QVector3D &pAmbient)
{
    mAmbient = pAmbient;
}

inline QVector3D PointLight::getDiffuse() const
{
    return mDiffuse;
}

inline void PointLight::setDiffuse(const QVector3D &pDiffuse)
{
    mDiffuse = pDiffuse;
}

inline QVector3D PointLight::getSpecular() const
{
    return mSpecular;
}

inline void PointLight::setSpecular(const QVector3D &pSpecular)
{
    mSpecular = pSpecular;
}

inline QVector3D PointLight::getViewPosition() const
{
    return mViewPosition;
}

inline void PointLight::setViewPosition(const QVector3D &pViewPosition)
{
    mViewPosition = pViewPosition;
}

inline QVector2D PointLight::getAttenuations() const
{
    return mAttenuations;
}

inline void PointLight::setAttenuations(const QVector2D &pAttenuations)
{
    mAttenuations = pAttenuations;
}

#endif // LIGHT_HPP
