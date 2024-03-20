#ifndef FOLLOWMAINCAMERACONTROLLER_HPP
#define FOLLOWMAINCAMERACONTROLLER_HPP

#include <camera/cameracontroller.hpp>
#include <rendering/rendercontext.hpp>
#include <system.hpp>

class FollowMainCamController: public CameraController {
public:
    FollowMainCamController(Camera pCamera): CameraController(pCamera) {}
    virtual void doControl() override {
        auto lMainCamera = ECS.get<RenderContext>(GameSystem::getMainRenderContext()).getCamera();
        getCamera().setViewMatrix(lMainCamera.getViewMatrix());
        getCamera().setProjectionMatrix(lMainCamera.getProjectionMatrix());
    }
};

#endif // FOLLOWMAINCAMERACONTROLLER_HPP
