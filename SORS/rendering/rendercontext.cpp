//
// Created by benne on 23.06.23.
//

#include "rendercontext.hpp"
entt::entity RenderContext::sActiveContextEnt;

void RenderContext::setUniforms()
{
    auto lCam = ECS.get<Camera>(mCamera);
    mActiveShader.shaderProgram()->setUniformValue("viewMatrix", lCam.getViewMatrix());
    mActiveShader.shaderProgram()->setUniformValue("projectionMatrix", lCam.getProjectionMatrix());
}

std::shared_ptr<RenderTarget> RenderContext::getTarget() const
{
    return mTarget;
}

RenderType RenderContext::getType() const
{
    return mType;
}

void RenderContext::setTarget(const std::shared_ptr<RenderTarget> &pTarget)
{
    mTarget = pTarget;
}


entt::entity RenderContext::getSActiveContextEnt()
{
    return sActiveContextEnt;
}

std::shared_ptr<Shader> RenderContext::getOwnShader() const
{
    return mOwnShader;
}

void RenderContext::setOwnShader(const std::shared_ptr<Shader> &pOwnShader)
{
    mOwnShader = pOwnShader;
}

void RenderContext::setType(RenderType pType)
{
    mType = pType;
}

unsigned int RenderContext::getPriority() const
{
    auto lPriority = ensurePriorityComponentisThere();
    return lPriority.mPriority;
}

void RenderContext::setPriority(unsigned int pPriority)
{
    auto &lPriority = ensurePriorityComponentisThere();
    lPriority.mPriority = pPriority;
}

void RenderContext::setActiveContextEnt(entt::entity pActiveContextEnt)
{
    sActiveContextEnt = pActiveContextEnt;
}

RenderContextPriority &RenderContext::ensurePriorityComponentisThere() const
{
    auto lEntToComponent = entt::to_entity(ECS, *this);
    Q_ASSERT(ECS.all_of<RenderContextPriority>(lEntToComponent));
    return(ECS.get<RenderContextPriority>(lEntToComponent));
}
