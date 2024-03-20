#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <QVector>
#include <ecs.hpp>
#include "irenderableproperty.hpp"

//! Jede Entity, die gerendert werden kann, hat diese Component. Wichtig ist hier das Verständnis,
//! dass das Renderable selbst keine Daten o.ä. hält. Das Renderable wird erst durch seine
//! Components definiert. So muss es immer eine Geometrie haben und kann durch Properties
//! genauer definiert werden.
class Renderable
{
public:
    Renderable();
    //! Hinfügen einer RenderableProperty zu einer Entity
    //! Für alle Properties wird preRender, render, postRender beim Renderdurchlauf
    //! für das aktuelle Entity gerufen
    void addProperty(std::shared_ptr<IRenderableProperty> pProperty);
    //! preRender wird beim Renderdurchlauf direkt nach dem Binden des passenden Shaders gerufen
    void preRender(Shader &pShader);
    //! render wird beim Renderdurchlauf direkt vor dem rendern der Geometrie gerufen
    //! Zwischen preRender und render werden die Transformationsmatrizen in system.cpp gesetzt
    void render(Shader &pShader);
    //! postRender wird nach dem Rendern der Geometrie gerufen, als letzte Operation des
    //! Renderdurchlaufs für ein Entity (es finden keine unbinds von Shader, etc. statt)
    void postRender(Shader &pShader);
    bool getIsEnabled() const;
    void setIsEnabled(bool pIsEnabled);

    bool getIsTransparent() const;
    void setIsTransparent(bool pIsTransparent);

    entt::entity getsingleRenderContext() const;
    void setSingleRenderContext(entt::entity pSingleRenderContext);

protected:
    QVector<std::shared_ptr<IRenderableProperty>> mProperties;
    entt::entity mSingleRenderContext{entt::null};
    bool mIsEnabled{true};
    bool mIsTransparent{false};
};

#endif // RENDERABLE_HPP
