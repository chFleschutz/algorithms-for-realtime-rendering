#include "system.hpp"
#include "geometry/geometrysystems.hpp"
#include "transformation/transformationsystems.hpp"
#include "rendering/renderingsystems.hpp"
#include "shadermanager.hpp"
#include "manager.hpp"
#include "rendering/rendercontext.hpp"
#include "rendering/renderable.hpp"
#include "geometry/geometrybase.hpp"

void GameSystem::mainLoop()
{
    auto lWidget = OpenGLWidget::getInstance();

    preRender();

    //Für den Mainrenderpass Mainrenderkontext setzen, Viewport aktualisieren
    RenderContext::setActiveContextEnt(GameSystem::getMainRenderContext());
    glFunctions->glViewport(0,
                            0,
                            lWidget->devicePixelRatio() * lWidget->width(),
                            lWidget->devicePixelRatio() * lWidget->height());
    render();

    postRender();
}

void GameSystem::preRender() {
    if (GameSystem::getMainRenderContext() == entt::null) {
        RenderContext::addRenderContext();
    }
    TransformationSystems::preRender();
    ShaderSystems::preRender();
    GeometrySystems::preRender();
    RenderingSystems::preRender();
}

void GameSystem::render()
{
    //Zunächst opake Objekte rendern mit Tiefenpuffer les- und schreibbar
    glFunctions->glDepthMask(GL_TRUE);
    doRender(false);
    //Dann transparente Objekte rendern mit Tiefenpuffer nur lesbar
    glFunctions->glDepthMask(GL_FALSE);
    doRender(true);
    glFunctions->glDepthMask(GL_TRUE);
}

void GameSystem::doRender(bool pTransparent)
{
    for (auto &&[lEntity, lShader, lRenderable]:
         ECS.view<Shader, Renderable>().use<Shader>().each())
    {
        auto lActiveContext = RenderContext::getActiveRenderContext();
        auto lSingleRenderContext = lRenderable.getsingleRenderContext();

        //Prerenderer können für Ihren Renderdurchlauf eigene Shader verwenden (z.B. AmbientOcclusion oder Shadowmap)
        Shader &lShaderToUse = (lActiveContext.getOwnShader() == nullptr)?
                                   lShader : (*lActiveContext.getOwnShader());

        //!Prüfen, ob das Renderabled temporär disabled ist und zum Durchlauf transparent/opak passt
        //! Außerdem prüfen, ob das Renderable für den aktuellen Kontext aktiv ist oder nur für einen (anderen) Kontext
        if (lRenderable.getIsEnabled() && (lRenderable.getIsTransparent() == pTransparent)
            && (lSingleRenderContext == entt::null || RenderContext::getSActiveContextEnt() == lSingleRenderContext))
        {
            auto i = 0;
            //Nur beim Wechseln Shaderprogramm (neu) laden
            auto lActShader = lActiveContext.getActiveShader();
            if ((i++ == 0) || (lActShader.getKey() != lShaderToUse.getKey())) {
                lActiveContext.setActiveShader(lShaderToUse);
                lShaderToUse.enableShader();
            }

            lRenderable.preRender(lShaderToUse);

            auto lGeo = GeometryBase::getGeometry(lEntity);
            Q_ASSERT(lGeo != nullptr);

            //Transformation Uniforms für Model- und Normalmatrix setzen
            QMatrix4x4 lMat;
            if (ECS.all_of<Transformation>(lEntity)) {
                auto lTrans = ECS.get<Transformation>(lEntity);
                lMat = lTrans.getWorldMatrix() * lTrans.getMatrix();
            }
            lMat *= lGeo->getmodelTransformation();
            lShaderToUse.shaderProgram()->setUniformValue("modelMatrix", lMat);
            auto lViewMat = lActiveContext.getCamera().getViewMatrix();
            lShaderToUse.shaderProgram()->setUniformValue("normalMatrix",
                                                     (lViewMat * lMat).normalMatrix());
            lActiveContext.setUniforms();
            lGeo->render();
            lRenderable.postRender(lShaderToUse);
        }
    }

}

void GameSystem::postRender() {
    TransformationSystems::postRender();
    ShaderSystems::postRender();
    GeometrySystems::postRender();
    RenderingSystems::postRender();

}

entt::entity GameSystem::getMainRenderContext()
{
    for (auto &&[lEntity, lRenderContext] : ECS.view<RenderContext>().each()) {
        if (lRenderContext.getType() == MainPass)
            return lEntity;
    }
    return RenderContext::addRenderContext();
}

