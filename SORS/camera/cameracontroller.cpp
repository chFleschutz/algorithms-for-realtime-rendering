#include "cameracontroller.hpp"
#include "manager.hpp"

CameraController::CameraController(Camera pCamera) :
    MoveController(),
    mYaw(0.f),
    mPitch(0.f),
    mRoll(0.f)
{
    mCamera = ECS.create();
    ECS.emplace<Camera>(mCamera, pCamera);
}

Camera &CameraController::getCamera()
{
    return ECS.get<Camera>(mCamera);
}
void CameraController::setCamera(Camera pCamera)
{
    ECS.emplace_or_replace<Camera>(mCamera, pCamera);
}
