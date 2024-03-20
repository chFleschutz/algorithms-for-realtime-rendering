#ifndef RENDERTOTEXTURE_H
#define RENDERTOTEXTURE_H

#include "irenderableproperty.hpp"
#include "rendering/rendertarget.hpp"
#include <rendering/rendercontext.hpp>

//! Verhalten der Kamera
enum RenderToTextureCameraBehavior { OwnCamera, FollowEntity, FollowMainCam};

class RenderToTexture : public IRenderableProperty
{
public:
    RenderToTexture(entt::entity pOwner,
                       RenderTargetType pType,
                       GLuint pTextureUnit = 3,
                    RenderToTextureCameraBehavior pCameraControlledbyEntity = OwnCamera,
                       QMatrix4x4 pCameraOffset = QMatrix4x4(),
                       QVector<TexRenderDestFormat> pTexImages = {},
                       RenderType pRenderType = PrePass,
                    bool pDisableOwnerForRendering = true);
    virtual ~RenderToTexture();
    //! Von IRenderableProperty abgeleitete Methoden implementieren
    virtual void init(Shader &shader) override;
    //! Rendering der Textur aktivieren
    virtual void preRender(Shader &shader) override;
    //! Rendering der Textur ausschalten
    virtual void postRender(Shader &shader) override;
    QMatrix4x4 getOffset() const;
    void setOffset(const QMatrix4x4 &pOffset);

    entt::entity getContext() const;
    void setContext(entt::entity pContext);

protected:
    entt::entity mContext;
    GLuint mTexDim;
    GLuint mTextureUnit;
    QVector<GLuint> mTexHandles;
    bool mCameraControlledbyEntity;
};

#endif // RENDERTOTEXTURE_H
