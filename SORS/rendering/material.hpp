#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "irenderableproperty.hpp"

//! Diese Klasse realisiert ein einfaches Material, das dann über Uniforms an die Shader übergeben wird.
//! Bei Anwendungen, die auf Performance Wert legen, würde man die Materialien und die Lichtquellen in
//! Feldern speichern, die dann in Uniform Buffer (UBO) an die Shader übergeben werden.
//! Diese einfache Property zeigt aber in einfacher Weise "wie es funktioniert".
class Material: public IRenderableProperty
{
    void preRender(Shader &pShader) override {
        pShader.shaderProgram()->setUniformValue("material.ambient", mAmbient);
        pShader.shaderProgram()->setUniformValue("material.diffuse", mDiffuse);
        pShader.shaderProgram()->setUniformValue("material.specular", mSpecular);
        pShader.shaderProgram()->setUniformValue("material.emission", mEmission);
        pShader.shaderProgram()->setUniformValue("material.shininess", mShininess);
    }

public:
    QVector4D getAmbient() const;
    void setAmbient(const QVector4D &pAmbient);
    QVector4D getDiffuse() const;
    void setDiffuse(const QVector4D &pDiffuse);
    QVector4D getSpecular() const;
    void setSpecular(const QVector4D &pSpecular);
    QVector4D getEmission() const;
    void setEmission(const QVector4D &pEmission);
    GLfloat getShininess() const;
    void setShininess(GLfloat pShininess);

protected:
    QVector4D mAmbient{0.3f,0.5f,0.3f,1.f};
    QVector4D mDiffuse{0.6f,0.8f,0.4f,1.f};
    QVector4D mSpecular{0.8f,0.8f,0.8f,1.f};
    QVector4D mEmission{0.f,0.f,0.f,1.f};
    GLfloat mShininess{80.f};
};

inline QVector4D Material::getAmbient() const
{
    return mAmbient;
}

inline void Material::setAmbient(const QVector4D &pAmbient)
{
    mAmbient = pAmbient;
}

inline QVector4D Material::getDiffuse() const
{
    return mDiffuse;
}

inline void Material::setDiffuse(const QVector4D &pDiffuse)
{
    mDiffuse = pDiffuse;
}

inline QVector4D Material::getSpecular() const
{
    return mSpecular;
}

inline void Material::setSpecular(const QVector4D &pSpecular)
{
    mSpecular = pSpecular;
}

inline QVector4D Material::getEmission() const
{
    return mEmission;
}

inline void Material::setEmission(const QVector4D &pEmission)
{
    mEmission = pEmission;
}

inline GLfloat Material::getShininess() const
{
    return mShininess;
}

inline void Material::setShininess(GLfloat pShininess)
{
    mShininess = pShininess;
}

#endif // MATERIAL_HPP
