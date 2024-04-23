#ifndef SSAONOISE_HPP
#define SSAONOISE_HPP

#include <random>
#include <cmath>
#include "rendering/irenderableproperty.hpp"

class SSAONoiseAndKernel: public IRenderableProperty {
public:
    SSAONoiseAndKernel(int unit = 0): mTextureUnit(unit)
    {
        // https://learnopengl.com/Advanced-Lighting/SSAO
        // TODO Samples für die Hemisphere erzeugen
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 64; ++i)
        {
            // TODO Samples in [-1;1] generieren, z in [0;1]

            // TODO sample verteilung anpassen: zufällig → größere Gewichtung in der Mitte


            //mSSAOKernel.push_back(sample);
        }

        // TODO generate noise texture (4x4 zufällig rotierte Vektoren um die tangent-space surface normal)


        // TODO einkommentieren wenn Code im Konstuktor passt
//        glFunctions->glGenTextures(1, &mTextureHandle);
//        glFunctions->glBindTexture(GL_TEXTURE_2D, mTextureHandle);
//        glFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, lSSAONoise.constData());
//        glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    virtual void preRender(Shader &shader) override
    {
        // TODO einkommentieren wenn Code im Konstuktor passt
//        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
//        glFunctions->glBindTexture(GL_TEXTURE_2D, mTextureHandle);
//        shader.shaderProgram()->setUniformValue(mUniformName.toStdString().c_str(), (GLuint)mTextureUnit);
//        //Flag setzen, an dem das Shaderprogramm erkennt, dass eine Textur vorhanden ist
//        shader.shaderProgram()->setUniformValue((QString("has").append(mUniformName)).toStdString().c_str(), true);
//        shader.shaderProgram()->setUniformValueArray("uSSAOKernel", mSSAOKernel.constData(), 64);
    }

    void postRender(Shader &shader) override
    {
        shader.shaderProgram()->setUniformValue((QString("has").append(mUniformName)).toStdString().c_str(), false);
        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
        glFunctions->glBindTexture(GL_TEXTURE_2D, 0);
    }

protected:
    unsigned int mTextureHandle;
    QString mUniformName{"SSAONoise"};
    int mTextureUnit{5};
    QVector<QVector3D> mSSAOKernel;
};

#endif // SSAONOISE_HPP
