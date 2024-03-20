#include "rendertotexture.hpp"
#include "rendercontext.hpp"
#include "ecs.hpp"
#include "camera/emptycameracontroller.hpp"
#include "camera/copycameracontroller.hpp"
#include <camera/followmaincameracontroller.hpp>

RenderToTexture::RenderToTexture(entt::entity pOwner,
                                       RenderTargetType pType,
                                       GLuint pTextureUnit,
                                 RenderToTextureCameraBehavior pCameraBehavior,
                                       QMatrix4x4 pCameraOffset, QVector<TexRenderDestFormat> pTexImages,
                                RenderType pRenderType, bool pDisableOwnerForRendering)
    : mTextureUnit(pTextureUnit)
    , mCameraControlledbyEntity(pCameraBehavior)
{
    if (pType == ColorCube)
        mTexDim = GL_TEXTURE_CUBE_MAP;
    else
        mTexDim = GL_TEXTURE_2D;

    switch (pCameraBehavior) {
    case FollowMainCam:
        mContext = RenderContext::addRenderContext<FollowMainCamController>(pRenderType);
        break;
    case OwnCamera:
        mContext = RenderContext::addRenderContext<EmptyCameraController>(pRenderType);
        break;
    case FollowEntity:
        mContext = RenderContext::addRenderContext<CopyCameraController>(pRenderType);
        const std::shared_ptr<CameraController> lCamController = ECS.get<RenderContext>(mContext)
                                                                     .getCameraController();
        std::dynamic_pointer_cast<CopyCameraController>(lCamController)->setEntity(pOwner);
        std::dynamic_pointer_cast<CopyCameraController>(lCamController)->setOffset(pCameraOffset);
        break;
    }

    auto lRenderTarget = std::make_shared<RenderTarget>(pOwner, pType, pTexImages, pDisableOwnerForRendering);
    ECS.get<RenderContext>(mContext).setTarget(lRenderTarget);
    auto &lCamera = ECS.get<RenderContext>(mContext).getCamera();
    //Kamera mit trivialen Einstellungen für Projektion
    lCamera.setAperture(90.0f);
    lCamera.setNearPlane(1.0f);
    lCamera.setFarPlane(1000.0f);
    lCamera.setAspect(lRenderTarget->getFboWidth() / lRenderTarget->getFboHeight());
    lCamera.setPosition(QVector3D(.0f, .0f, .0f));
    lCamera.update();
    mTexHandles = lRenderTarget->getTexHandles();
}

RenderToTexture::~RenderToTexture() {}

void RenderToTexture::init(Shader &shader)
{
}

void RenderToTexture::preRender(Shader &shader)
{
    for (auto i=0; i<mTexHandles.length(); i++) {
        QString lName;
        lName = QString("textureMap").append(QString::number(mTextureUnit+i));

        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit + i);
        glFunctions->glBindTexture(mTexDim, mTexHandles[i]);
        //Beim Mainpass Texturen benennen
        if (mContext != RenderContext::getSActiveContextEnt()) {
            shader.shaderProgram()->setUniformValue(lName.toStdString().c_str(), mTextureUnit + i);
            shader.shaderProgram()->setUniformValue((QString("has").append(lName)).toStdString().c_str(), true);
        }
    }
}

void RenderToTexture::postRender(Shader &shader)
{
    for (auto i=0; i<mTexHandles.length(); i++) {
        QString lName;
        lName = QString("textureMap").append(QString::number(mTextureUnit + i));
        shader.shaderProgram()->setUniformValue((QString("has").append(lName)).toStdString().c_str(), false);
        glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit + i);
        glFunctions->glBindTexture(mTexDim, 0);
    }
}

entt::entity RenderToTexture::getContext() const
{
    return mContext;
}

void RenderToTexture::setContext(entt::entity pContext)
{
    mContext = pContext;
}
