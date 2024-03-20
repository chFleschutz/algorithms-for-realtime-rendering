#ifndef MANDELBROTPROPERTY_HPP
#define MANDELBROTPROPERTY_HPP

#include "rendering/texture.hpp"
#include "input/inputregistry.hpp"

class MandelbrotProperty: public Texture
{
public:
    MandelbrotProperty(unsigned pSizeX, unsigned pSizeY) {mImage = std::make_shared<QImage>(pSizeX, pSizeY, QImage::Format_RGBA8888);}
    void preRender(Shader &pShader) override {
        auto keyIn = InputRegistry::getInstance().getKeyboardInput();
        if (keyIn->isKeyPressed('k'))
            mZoom *= 1.1f;
        else if (keyIn->isKeyPressed('i'))
            mZoom /= 1.1f;
        else if (keyIn->isKeyPressed('j'))
            mCenter+=QVector2D(-0.01f,0.0f);
        else if (keyIn->isKeyPressed('l'))
            mCenter+=QVector2D(0.01f,0.0f);
        else if (keyIn->isKeyPressed('u'))
            mCenter+=QVector2D(0.0f,0.01f);
        else if (keyIn->isKeyPressed('o'))
            mCenter-=QVector2D(0.0f,0.01f);
        else if (keyIn->isKeyPressed('+'))
            mMaxIterations *= 1.2f;
        else if (keyIn->isKeyPressed('-'))
            mMaxIterations /= 1.2f;
        else if (keyIn->isKeyPressed('1'))
            mRenderWithShader = true;
        else if (keyIn->isKeyPressed('2'))
            mRenderWithShader = false;

        pShader.shaderProgram()->setUniformValue("MaxIterations", mMaxIterations);
        pShader.shaderProgram()->setUniformValue("Zoom", mZoom);
        pShader.shaderProgram()->setUniformValue("Center", mCenter);
        pShader.shaderProgram()->setUniformValue("InnerColor", mInnerColor);
        pShader.shaderProgram()->setUniformValue("OuterColor1", mOuterColor1);
        pShader.shaderProgram()->setUniformValue("OuterColor2", mOuterColor2);
        pShader.shaderProgram()->setUniformValue("RenderWithShader", mRenderWithShader);

        if (!mRenderWithShader)
        {
            Texture::preRender(pShader);
            calculate();
        }
    }

    void calculate()
    {
        float   lReal = 0.0f;
        float   lImag = 0.0f;
        float   lCreal = 0.0f;
        float   lCimag = 0.0f;

        float lR2 = 0.0;
        float lIter;

        int lSizex = mImage->width();
        int lSizey = mImage->height();

        for (auto x=0; x<lSizex; x++) //Schleife über alle Pixel
        {
            for (auto y=0; y<lSizey; y++)
            {
                QColor lColor;

                lReal  = (x/float(lSizex)-0.5) * mZoom + mCenter.x();
                lImag  = (y/float(lSizey)-0.5) * mZoom + mCenter.y();
                lCreal = lReal;
                lCimag = lImag;

                for (lIter = lR2 = 0.0f; (lIter < mMaxIterations) && (lR2 < 4.0f); ++lIter)
                {
                    float tempreal = lReal;

                    lReal = (tempreal * tempreal) - (lImag * lImag) + lCreal;
                    lImag = 2.0 * tempreal * lImag + lCimag;
                    lR2   = (lReal * lReal) + (lImag * lImag);
                }

                // Base the color on the number of iterations

                if (lR2 < 4.0)
                    lColor = QColor(mInnerColor[0]*255, mInnerColor[1]*255, mInnerColor[2]*255);
                else
                {
                    float t = lIter*0.05f - int(lIter*0.05f);
                    QVector3D lInterpolatedColor = mOuterColor1*(1.0f-t) + mOuterColor2*t;
                    lColor = QColor(lInterpolatedColor.x()*255, lInterpolatedColor.y()*255, lInterpolatedColor.z()*255);
                }
                mImage->setPixelColor(x,y,lColor);
            }
        }
        this->loadImageToGPU();
    }

protected:
    bool mRenderWithShader{false};
    float mMaxIterations{10.0f};
    float mZoom{1.0f};
    QVector2D mCenter{0.0f,0.0f};
    QVector3D mInnerColor{1.0f,0.0f,0.0f};
    QVector3D mOuterColor1{0.0f,1.0f,0.0f};
    QVector3D mOuterColor2{0.0f,0.0f,1.0f};
};

#endif // MANDELBROTPROPERTY_HPP
