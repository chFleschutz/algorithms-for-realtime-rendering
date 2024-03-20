#include "renderingsystems.hpp"
#include "ecs.hpp"
#include "rendercontext.hpp"
#include "renderable.hpp"

void RenderingSystems::preRender()
{
    doFilteredAndSortedRender(PrePass);
}

void RenderingSystems::postRender()
{
    doFilteredAndSortedRender(PostPass);
}

void RenderingSystems::deactivateAll()
{
    for (auto &&[lEntity, lRenderable] : ECS.view<Renderable>().each()) {
        lRenderable.setIsEnabled(false);
    }
}

void RenderingSystems::doFilteredAndSortedRender(RenderType pType)
{
    ECS.sort<RenderContextPriority>([](const auto &lhs, const auto &rhs) {
        return lhs.mPriority < rhs.mPriority;
    });

    //for (auto &&[lEntity, lRenderContext] : ECS.view<RenderContext>().each()) {
    for (auto &&[lEntity, lRenderContext, lRenderContextPriority] : ECS.view<RenderContext, RenderContextPriority>().use<RenderContextPriority>().each()) {
        if (lRenderContext.getType() == pType) {
            RenderContext::setActiveContextEnt(lEntity);
            lRenderContext.getTarget()->render();
        }
    }
}
