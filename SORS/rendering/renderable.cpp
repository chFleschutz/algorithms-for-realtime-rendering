#include "renderable.hpp"

#include <QVector>

Renderable::Renderable(): mProperties()
{

}

void Renderable::addProperty(std::shared_ptr<IRenderableProperty> pProperty)
{
    mProperties.append(pProperty);
}

void Renderable::preRender(Shader &pShader)
{
    for (auto &lProperty: mProperties)
        lProperty->preRender(pShader);
}

void Renderable::render(Shader &pShader)
{
    for (auto &lProperty: mProperties)
        lProperty->render(pShader);
}

void Renderable::postRender(Shader &pShader)
{
    for (auto &lProperty: mProperties)
        lProperty->postRender(pShader);
}

bool Renderable::getIsEnabled() const
{
    return mIsEnabled;
}

void Renderable::setIsEnabled(bool pIsEnabled)
{
    mIsEnabled = pIsEnabled;
}

bool Renderable::getIsTransparent() const
{
    return mIsTransparent;
}

void Renderable::setIsTransparent(bool pIsTransparent)
{
    mIsTransparent = pIsTransparent;
}

entt::entity Renderable::getsingleRenderContext() const
{
    return mSingleRenderContext;
}

void Renderable::setSingleRenderContext(entt::entity pSingleRenderContext)
{
    mSingleRenderContext = pSingleRenderContext;
}
