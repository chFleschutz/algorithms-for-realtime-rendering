#ifndef RENDERTARGET_HPP
#define RENDERTARGET_HPP

#include "opengl.hpp"
#include "ecs.hpp"
#include "textureimageformat.hpp"

//! Beschreibung der Texturen und des/der Targets, in die gerendert werden soll
struct TexRenderDestFormat {
    TexImageFormat imageFormat;
    GLuint renderDest;   //!< Attachment, das benutzt werden soll, z.B. GL_COLOR_ATTACHMENT0
};

enum RenderTargetType { Color2D, ColorCube, DepthOnly };

//! Stellt ein FBO zur Verfügung, das zusammen mit einem RenderContext für einen (typischerweise Pre-) Renderpass
//! verwendet werden kann. Ein RenderTarget gehört immer zu einem RenderContext. Die Methoden des RenderTarget
//! dürfen nur gerufen werden, wenn der zugehörige Kontext aktiv ist.
//! Es werden alle im ECS vorhandenen Renderables gerendert. Durch verschiedene Storage
//! classes oder andere Tags (z.B. Zuordnung der Renderables zu nummerierten RenderContext) könnte eine
//! Trennung erreicht werden.
//! Das Renderable, an dem eine Property hängt, die das Ergebnis des Prerenderpasses (Textur) enthält, wird
//! beim Rendern deaktiviert.
class RenderTarget
{
public:
    RenderTarget(entt::entity pOwnerRenderable, RenderTargetType pType, QVector<TexRenderDestFormat> pTexImages, bool pDisableOwnerForRendering);
    //! Getter für die Id der Textur in die das FBO rendert.
    virtual QVector<GLuint> getTexHandles() {return (mTexHandles);}
    void render();
    ~RenderTarget();
    GLsizei getFboWidth() const;
    void setFboWidth(GLsizei pFboWidth);

    GLsizei getFboHeight() const;
    void setFboHeight(GLsizei pFboHeight);

protected:
    virtual void initColorBuffer();
    virtual void initDepthBuffer();

private:
    entt::entity mOwnerRenderable{entt::null};
    bool mDisableOwnerForRendering{true};
    GLuint mFBO{0};                        //!< Handle auf FBO
    GLuint mRenderbuffer{0};               //!< Handle zu Renderbuffer (Speicher), der an das FBO gebunden wird
    //!< Daten der Textur, in die gerendert wird.
    QVector<TexRenderDestFormat> mTexImages{{GL_RGBA, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0}};
    QVector<GLuint> mTexHandles;        //!< Platz für die GL-Handles zu den TexImages
    GLuint mTexDim;                     //!< Dimension der Textur, in die gerendert wird.
    bool mValid{false};                        //!< gibt an ob das FBO korrekt erstellt und gebunden werden konnte

    GLsizei mFboWidth{1024}; //!< Die horizontale Auflösung des FBOs
    GLsizei mFboHeight{1024}; //!< Die vertikale Auflösung des FBOs

    //! Überprüft ob das Framebuffer-Object richtig erstellt und gebunden wurde.
    bool checkFramebufferStatus();
    void initFrameBufferAndRenderTarget();
};

#endif // RENDERTARGET_HPP
