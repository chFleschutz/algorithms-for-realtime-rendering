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
        auto lCamera = ECS.get<RenderContext>(getContext()).getCamera();
        RenderToTexture::preRender(shader);
        shader.shaderProgram()->setUniformValue("MVPLight", lCamera.getProjectionMatrix() * lCamera.getViewMatrix());
        shader.shaderProgram()->setUniformValue("modelViewLight", lCamera.getViewMatrix());
        shader.shaderProgram()->setUniformValue("normalMatrixLight", lCamera.getViewMatrix().normalMatrix());
        if (mContext == RenderContext::getSActiveContextEnt())
            shader.shaderProgram()->setUniformValue("buildingShadowmap", true);
        else
            shader.shaderProgram()->setUniformValue("buildingShadowmap", false);
    }

private:
};

#endif // SHADOWMAPTEXTURE_HPP
