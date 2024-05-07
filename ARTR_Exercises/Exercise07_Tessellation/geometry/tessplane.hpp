#ifndef TESSPLANE_HPP
#define TESSPLANE_HPP

#include "geometry/geometrybase.hpp"
#include "ecs.hpp"
#include "geometry/igeometryimplementation.hpp"
#include "geometry/simpleplane.hpp"
#include "input/inputregistry.hpp"
#include "rendering/rendercontext.hpp"

class TessPlane: public SimplePlane
{
public:
    TessPlane(float pSizeX, float pSizeY): SimplePlane(pSizeX, pSizeY, 1) {}
protected:

    void keyboard()
    {
        auto keyIn = InputRegistry::getInstance().getKeyboardInput();

        if (keyIn->isKeyPressed('1'))
            mWaterLevel -= 0.001f;
        else if (keyIn->isKeyPressed('2'))
            mWaterLevel += 0.001f;
        else if (keyIn->isKeyPressed('3'))
            mSandLevel -= 0.001f;
        else if (keyIn->isKeyPressed('4'))
            mSandLevel += 0.001f;
        else if (keyIn->isKeyPressed('5'))
            mGrassLevel -= 0.001f;
        else if (keyIn->isKeyPressed('6'))
            mGrassLevel += 0.001f;

        if(mWaterLevel > mSandLevel)
            mWaterLevel = mSandLevel-0.01f;
        if(mWaterLevel < 0.0f)
            mWaterLevel = 0.0f;
        if(mSandLevel > mGrassLevel)
            mSandLevel = mGrassLevel-0.01f;
        if(mSandLevel < mWaterLevel)
            mSandLevel = mWaterLevel+0.01f;
        if(mGrassLevel > 1.0f)
            mGrassLevel = 1.0f;
        if(mGrassLevel < mSandLevel)
            mGrassLevel = mSandLevel+0.01f;


    }

    void fillVertices(QVector<QVector4D>& vertices) override
    {
        float width = mXSize;
        float height = mYSize;
        vertices.clear();
        // Tri one
        vertices.append(QVector4D(-0.5f * width, -0.5f * height, 0.f, 1.0f));
        vertices.append(QVector4D(0.5f * width, -0.5f * height, 0.f, 1.0f));
        vertices.append(QVector4D(-0.5f * width, 0.5f * height, 0.f, 1.0f));
        vertices.append(QVector4D(-0.5f * width, 0.5f * height, 0.f, 1.0f));
        vertices.append(QVector4D(0.5f * width, -0.5f * height, 0.f, 1.0f));
        vertices.append(QVector4D(0.5f * width, 0.5f * height, 0.f, 1.0f));
    }

    void fillTexCoords(QVector<QVector2D>& texCoords) override
    {
        texCoords.clear();
        texCoords.append(QVector2D(0.0, 0.0));
        texCoords.append(QVector2D(1.0, 0.0));
        texCoords.append(QVector2D(0.0, 1.0));
        texCoords.append(QVector2D(0.0, 1.0));
        texCoords.append(QVector2D(1.0, 0.0));
        texCoords.append(QVector2D(1.0, 1.0));
    }

    virtual void render() override
    {
        keyboard();
        auto lShaderProgram = RenderContext::getActiveRenderContext().getActiveShader();
        lShaderProgram.shaderProgram()->setPatchVertexCount(3);
        lShaderProgram.shaderProgram()->setUniformValue("waterLevel", mWaterLevel);
        lShaderProgram.shaderProgram()->setUniformValue("sandLevel", mSandLevel);
        lShaderProgram.shaderProgram()->setUniformValue("grassLevel", mGrassLevel);
        glFunctions->glDrawArrays(GL_PATCHES, 0, 6);
    }
    float mWaterLevel = 0.15f;
    float mSandLevel = 0.3f;
    float mGrassLevel = 0.5f;
};

#endif // TESSPLANE_HPP
