#include "rendertarget.hpp"
#include "rendering/renderable.hpp"
#include "transformation/transformation.hpp"
#include "rendering/rendercontext.hpp"
#include "system.hpp"
#include "win_oglbase/openglwidget.hpp"

RenderTarget::RenderTarget(entt::entity pOwnerRenderable, RenderTargetType pType,
                            QVector<TexRenderDestFormat> pTexImages = {},
                            bool pDisableOwnerForRendering = true):
    mOwnerRenderable(pOwnerRenderable), mDisableOwnerForRendering(pDisableOwnerForRendering)
{
    mTexDim = GL_TEXTURE_2D;
    switch (pType) {
        case Color2D:
            break;
        case ColorCube:
            mTexDim = GL_TEXTURE_CUBE_MAP;
            break;
        case DepthOnly:
            mTexImages[0].imageFormat.mFormat = GL_DEPTH_COMPONENT;
            mTexImages[0].imageFormat.mInternalFormat = GL_DEPTH_COMPONENT;
            mTexImages[0].imageFormat.mDataType = GL_FLOAT;
            mTexImages[0].renderDest = GL_DEPTH_ATTACHMENT;
            break;
        default :
            qDebug() << "Rendertarget got invalid Targettype.";
            break;
    }

    if (!pTexImages.empty())
        mTexImages = pTexImages;

    initFrameBufferAndRenderTarget();
    if (mTexImages[0].renderDest != GL_DEPTH_ATTACHMENT)
        initColorBuffer();
    else
    {
        initDepthBuffer();
        glFunctions->glDrawBuffer(GL_NONE); //Kein Drawbuffer;
    }

    mValid = checkFramebufferStatus();

    if (!mValid)
    {
        qDebug() << "For " << ((mTexDim == GL_TEXTURE_2D) ? "2D-Texture or Depthmap" : "Cube-Texture");
        qDebug() << "Framebuffer not properly set up.";
    }
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderTarget::initColorBuffer()
{
    QVector<unsigned int> lDrawBuffers;

    // Renderbuffer erzeugen und binden
    glFunctions->glGenRenderbuffers(1, &mRenderbuffer);
    glFunctions->glBindRenderbuffer(GL_RENDERBUFFER, mRenderbuffer);
    //Tiefenpuffer erzeugen
    glFunctions->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mFboWidth, mFboHeight);
    //Tiefenpuffer an FB hängen
    glFunctions->glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                           GL_DEPTH_ATTACHMENT,
                                           GL_RENDERBUFFER,
                                           mRenderbuffer);

    for (auto i = 0; i < mTexImages.length(); i++) {
        //Texturehandle erzeugen
        glFunctions->glBindTexture(mTexDim, mTexHandles[i]);
        if (mTexDim == GL_TEXTURE_CUBE_MAP) {
            //Bei Cubemaps brauchen wir sechs Texturen, eine für jede Würfelseite
            glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            for (int j = 0; j < 6; j++) {
                glFunctions->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j,
                                          0,
                                          mTexImages[i].imageFormat.mInternalFormat,
                                          mFboWidth,
                                          mFboHeight,
                                          0,
                                          mTexImages[i].imageFormat.mFormat,
                                          mTexImages[i].imageFormat.mDataType,
                                          0);
            }
        } else {
            glFunctions->glTexImage2D(mTexDim,
                                      0,
                                      mTexImages[i].imageFormat.mInternalFormat,
                                      mFboWidth,
                                      mFboHeight,
                                      0,
                                      mTexImages[i].imageFormat.mFormat,
                                      mTexImages[i].imageFormat.mDataType,
                                      0);
            glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER, mTexImages[i].renderDest, GL_TEXTURE_2D, mTexHandles[i], 0);
        }
    }
    //OpenGL die Drawbuffers nennen
    for (auto i = 0; i < mTexImages.length(); i++) {
        lDrawBuffers.append(mTexImages[i].renderDest);
    }
    glFunctions->glDrawBuffers(mTexImages.length(), lDrawBuffers.data());
}

void RenderTarget::initDepthBuffer()
{
    glFunctions->glTexImage2D(mTexDim,
                              0,
                              mTexImages[0].imageFormat.mInternalFormat,
                              mFboWidth,
                              mFboHeight,
                              0,
                              mTexImages[0].imageFormat.mFormat,
                              mTexImages[0].imageFormat.mDataType,
                              0);
    glFunctions->glFramebufferTexture(GL_FRAMEBUFFER, mTexImages[0].renderDest, mTexHandles[0], 0);
}

GLsizei RenderTarget::getFboHeight() const
{
    return mFboHeight;
}

void RenderTarget::setFboHeight(GLsizei pFboHeight)
{
    mFboHeight = pFboHeight;
}

GLsizei RenderTarget::getFboWidth() const
{
    return mFboWidth;
}

void RenderTarget::setFboWidth(GLsizei pFboWidth)
{
    mFboWidth = pFboWidth;
}

void RenderTarget::initFrameBufferAndRenderTarget()
{
    // FBO erzeugen und binden
    glFunctions->glGenFramebuffers(1, &mFBO);
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    mTexHandles.resize(mTexImages.length());
    // Textur(en) als Rendertarget erzeugen
    for (auto i = 0; i < mTexImages.length(); i++) {
        glFunctions->glGenTextures(1, &mTexHandles[i]);
        glFunctions->glBindTexture(mTexDim, mTexHandles[i]);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glFunctions->glTexParameterf(mTexDim, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

//! Rendert Szene in das FBO.
void RenderTarget::render()
{
    if (!mValid) {
        qWarning() << "FBO Renderer called with non-valid FBO.";
        return;
    }
    glFunctions->glViewport(0, 0, mFboWidth, mFboHeight);

    // Das Drawable auf dass das FBO gerendert werden soll deaktivieren damit das bild nicht von ihm überdeckt wird!
    auto &lOwnerRenderable = ECS.get<Renderable>(mOwnerRenderable);
    QMatrix4x4 lOwnerViewMatrix;

    //Viewmatrix des Renderables holen, an dem das FBO hängt
    if (ECS.all_of<Transformation>(mOwnerRenderable)) {
        auto lTrans = ECS.get<Transformation>(mOwnerRenderable);
        lOwnerViewMatrix = lTrans.getWorldMatrix() * lTrans.getMatrix();
    }

    if (mDisableOwnerForRendering)
        lOwnerRenderable.setIsEnabled(false);

    // FBO binden, um darin zu rendern
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glFunctions->glEnable(GL_DEPTH_TEST);
    glFunctions->glClearColor(0.0, 0.0, 0.0, 0.0);

    if (mTexDim == GL_TEXTURE_2D) {
        glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GameSystem::render();
    } else // Cubemap
    {
        auto &lCam = RenderContext::getActiveRenderContext().getCamera();
        //Wir holen uns hier die Position des Objektes, an dem die Cubemap hängen soll
        //Genau genommen müsste man sich auch noch etwaige Rotationen holen, damit das Ergebnis wirklich richtig ist
        //Das ist hier ausgelassen, d.h., sobald man das "Empfängerobjekt" rotiert, stimmt das Ergebnis nicht mehr
        lCam.setPosition(lOwnerViewMatrix.column(3).toVector3D());

        for (auto i = 0; i < mTexImages.length(); i++) {
            for (int j = 0; j < 6; j++) {
                lCam.resetRotation();
                glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER,
                                                    mTexImages[i].renderDest,
                                                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + j,
                                                    mTexHandles[i],
                                                    0);
                // Kamera für 6 verschiedene Richtungen setzen
                switch (GL_TEXTURE_CUBE_MAP_POSITIVE_X + j) {
                case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
                    lCam.setRotation(90.0f,0.0f,0.0f);
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
                    lCam.setRotation(-90.0f, 0.0f, 0.0f);
                    break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
                    lCam.setRotation(0.0f,90.0f,0.0f);
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
                    lCam.setRotation(0.0f,-90.0f,0.0f);
                    break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
                    lCam.setRotation(0.0f,0.0f,.0f);
                    break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
                    lCam.setRotation(180.0f,0.0f,.0f);
                    break;
                }
                lCam.update();
                // Objekte rendern
                glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                GameSystem::render();
            }
        }
    }

    // Rendern in die Textur(en) beenden
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, OpenGLWidget::getInstance()->getContext()->defaultFramebufferObject());
    glFunctions->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFunctions->glBindTexture(mTexDim, 0);

    // Das Drawable, "auf das" das FBO gerendert werden soll wieder aktivieren, falls es deaktiviert war
    if (mDisableOwnerForRendering)
        lOwnerRenderable.setIsEnabled(true);
}

RenderTarget::~RenderTarget()
{
    if (mRenderbuffer)
    {
        glFunctions->glDeleteRenderbuffers(1, &mRenderbuffer);
    }
    if (mFBO)
    {
        glFunctions->glDeleteFramebuffers(1, &mFBO);
    }
    for (auto ltexHandles: mTexHandles)
    {
        glFunctions->glDeleteTextures(1, &(ltexHandles));
    }
}

bool RenderTarget::checkFramebufferStatus()
{
    GLenum status = (GLenum)glFunctions->glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        qDebug() << "Framebuffer incomplete, incomplete attachment\n";
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        qDebug() << "Unsupported framebuffer format\n";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        qDebug() << "Framebuffer incomplete, missing attachment\n";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        qDebug() << "Framebuffer incomplete, attached images must have same dimensions\n";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        qDebug() << "Framebuffer incomplete, attached images must have same format\n";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        qDebug() << "Framebuffer incomplete, missing draw buffer\n";
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        qDebug() << "Framebuffer incomplete, missing read buffer\n";
        return false;
    }
    return false;
}
