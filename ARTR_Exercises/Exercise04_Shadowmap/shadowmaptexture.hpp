#ifndef SHADOWMAPTEXTURE_HPP
#define SHADOWMAPTEXTURE_HPP

#include <rendering/rendertotexture.hpp>
#include <camera/camera.hpp>
#include <rendering/rendercontext.hpp>

class ShadowMapTexture : public RenderToTexture
{
public:
    ShadowMapTexture(entt::entity pOwner,
                     RenderTargetType pType,
                     GLuint pTextureUnit = 3,
                     RenderToTextureCameraBehavior pCameraControlledbyEntity = OwnCamera,
                     QMatrix4x4 pCameraOffset = QMatrix4x4())
        : RenderToTexture(pOwner, pType, pTextureUnit, pCameraControlledbyEntity, pCameraOffset)
    {
        auto &lCamera = ECS.get<RenderContext>(mContext).getCamera();
        lCamera.setProjectionMatrix(QMatrix4x4());
        lCamera.setAperture(90.0f);
        lCamera.setNearPlane(1.0f);
        lCamera.setFarPlane(1000.0f);
        lCamera.update();
    }

    virtual void preRender(Shader &shader) override
    {
        keyboard();

        auto& lCamera = ECS.get<RenderContext>(getContext()).getCamera();
        RenderToTexture::preRender(shader);
        shader.shaderProgram()->setUniformValue("MVPLight", lCamera.getProjectionMatrix() * lCamera.getViewMatrix());
        shader.shaderProgram()->setUniformValue("modelViewLight", lCamera.getViewMatrix());
        shader.shaderProgram()->setUniformValue("normalMatrixLight", lCamera.getViewMatrix().normalMatrix());
        if (mContext == RenderContext::getSActiveContextEnt())
            shader.shaderProgram()->setUniformValue("buildingShadowmap", true);
        else
            shader.shaderProgram()->setUniformValue("buildingShadowmap", false);

        switch (mSoftShadowMode)
        {
        default:
        case ShadowMapTexture::SoftShadowMode::None:
            shader.shaderProgram()->setUniformValue("softShadowMode", 0);
            break;
        case ShadowMapTexture::SoftShadowMode::PCF:
            shader.shaderProgram()->setUniformValue("softShadowMode", 1);
            break;
        case ShadowMapTexture::SoftShadowMode::Poisson:
            shader.shaderProgram()->setUniformValue("softShadowMode", 2);
            break;
        }
    }

private:
    enum class SoftShadowMode
    {
		None,
		PCF,
		Poisson,
	};

    void keyboard()
    {
        auto keyIn = InputRegistry::getInstance().getKeyboardInput();
        if (keyIn->isKeyPressed('1'))
        {
            mSoftShadowMode = SoftShadowMode::None;
        }
		else if (keyIn->isKeyPressed('2'))
        {
            mSoftShadowMode = SoftShadowMode::PCF;
        }
		else if (keyIn->isKeyPressed('3'))
        {
            mSoftShadowMode = SoftShadowMode::Poisson;
        }
    }

    SoftShadowMode mSoftShadowMode = SoftShadowMode::None;
};

#endif // SHADOWMAPTEXTURE_HPP
