#ifndef VBUFFER_HPP
#define VBUFFER_HPP
#include "rendering/irenderableproperty.hpp"
#include "rendering/rendercontext.hpp"
#include "geometry/geometrybase.hpp"
#include "transformation/transformation.hpp"

class VBufferUniforms: public IRenderableProperty {
public:
    VBufferUniforms(entt::entity pOwnerEntity): mOwnerEntity(pOwnerEntity) {}

    void preRender(Shader &pShader) override {

        //TODO Refactor Methode schreiben, die in Gamesystems::doRender und hier verwendete werden kann
        //vmtl als Methode von Renderable?
        auto& lCamera = RenderContext::getActiveRenderContext().getCamera();
        pShader.shaderProgram()->setUniformValue("UoldMVP", mLastMVP);
        auto lGeo = GeometryBase::getGeometry(mOwnerEntity);
        Q_ASSERT(lGeo != nullptr);

        //Transformation Uniforms für Model- und Normalmatrix setzen
        QMatrix4x4 lMat;
        if (ECS.all_of<Transformation>(mOwnerEntity)) {
            auto& lTrans = ECS.get<Transformation>(mOwnerEntity);
            lMat = lTrans.getWorldMatrix() * lTrans.getMatrix();
        }
        lMat *= lGeo->getmodelTransformation();

        mLastMVP = lCamera.getProjectionMatrix() * lCamera.getViewMatrix() *lMat;
    }
protected:
    entt::entity mOwnerEntity;
    QMatrix4x4 mLastMVP;
};

#endif // VBUFFER_HPP
