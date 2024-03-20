//
// Created by benne on 23.06.23.
//

#ifndef CGUEBUNGEN_RENDERCONTEXT_HPP
#define CGUEBUNGEN_RENDERCONTEXT_HPP

#include "ecs.hpp"
#include "camera/camera.hpp"
#include "shader.hpp"
#include "shadermanager.hpp"
#include "camera/mousekeyboardcameracontroller.hpp"
#include "win_oglbase/openglwidget.hpp"
#include "rendertarget.hpp"

//! Verschiedene Typen der Renderer
enum RenderType { PrePass, MainPass, PostPass};

struct RenderContextPriority {
    unsigned int mPriority{1};
};

//! Ein RenderContext hält alle Informationen, die für einen Renderdurchlauf über die Entities hinaus
//! wichtig sind. Das ist immer mindestens eine Kamera. Darüber hinaus können es verschiedene Rendertargets
//! oder die Konfiguration der Buffer, in die gerendert werden soll, sein.
class RenderContext {

public:
    RenderContext(const entt::entity pCamera, std::shared_ptr<CameraController> pController, RenderType pType = MainPass) :
        mCamera(pCamera), mCameraController(pController), mActiveShader(*ShaderManager::getDefaultShader()),
        mType(pType) {}
    RenderContext() = delete;
    Camera &getCamera() {return ECS.get<Camera>(mCamera);}
    void setCamera(const Camera &pCamera){ ECS.emplace_or_replace<Camera>(mCamera, pCamera);}
    Shader& getActiveShader() const {return mActiveShader;}
    void setActiveShader(Shader& pActiveShader) {mActiveShader = pActiveShader;}

    template <typename T = MouseKeyboardCameraController>
    static entt::entity addRenderContext(RenderType pType = MainPass) {
        Camera lCam;
        sActiveContextEnt = ECS.create();
        static_assert(std::is_base_of<CameraController, T>::value, "Controller must be derived from CameraController");
        auto lCoCam = std::make_shared<T>(lCam);
        OpenGLWidget::getInstance()->addTickObject(lCoCam);
        auto &lContext = ECS.emplace<RenderContext>(sActiveContextEnt, lCoCam->getCameraEnt(), lCoCam, pType);
        auto &lPrio = ECS.emplace<RenderContextPriority>(sActiveContextEnt);
        return(sActiveContextEnt);
    }

    static RenderContext& getActiveRenderContext() {return ECS.get<RenderContext>(RenderContext::sActiveContextEnt);}
    const std::shared_ptr<CameraController> &getCameraController() const {return mCameraController;}
    void setUniforms();

    std::shared_ptr<RenderTarget> getTarget() const;
    RenderType getType() const;

    void setTarget(const std::shared_ptr<RenderTarget> &pTarget);

    static void setActiveContextEnt(entt::entity pActiveContextEnt);

    static bool getSRendererActive();
    static void setSRendererActive(bool pSRendererActive);

    static entt::entity getSActiveContextEnt();

    std::shared_ptr<Shader> getOwnShader() const;
    void setOwnShader(const std::shared_ptr<Shader> &newOwnShader);

    void setType(RenderType pType);

    unsigned int getPriority() const;
    void setPriority(unsigned int pPriority);

protected:
    static entt::entity sActiveContextEnt;
    entt::entity mCamera;
    std::shared_ptr<CameraController> mCameraController;
    Shader& mActiveShader;
    //!In welchem Pass wird der Kontext verwendet
    RenderType mType{MainPass};
    //!Mit welcher Priorität wird der Kontext gerendert, d.h. die Renderkontexte einer Klasse (Pre, Main, Post) werden sortiert ausgeführt
    std::shared_ptr<RenderTarget> mTarget{nullptr};
    //! Für manche Pre-/Postrendering-Durchläufe werden andere Shader, als die von den Objekten vorgesehen verwendet
    //! Siehe z.B. AmbientOcclusion, auch Shadowmaps könn(t)en so realisiert werden
    std::shared_ptr<Shader> mOwnShader{nullptr};
    //! Sicherstellen, dass Priority-Component vorhanden ist, sonst hinzufügen
    RenderContextPriority& ensurePriorityComponentisThere() const;
};

#endif //CGUEBUNGEN_RENDERCONTEXT_HPP
