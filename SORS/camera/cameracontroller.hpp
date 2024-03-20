#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "thirdparty/entt/single_include/entt/entt.hpp"

#include "camera/camera.hpp"
#include "transformation/movecontroller.hpp"

//! Auch die Kamera kann mit den üblichen MoveController kontrolliert werden. Diese Klasse stellt die Verbindung
//! zwischen Kamera und Controller her.
class CameraController : public MoveController
{
public:
    CameraController(Camera pCamera);

    Camera &getCamera();
    entt::entity getCameraEnt() {return mCamera;}
    void setCamera(Camera pCamera);

protected:
    entt::entity mCamera;

    float mYaw{0.f};
    float mPitch{0.f};
    float mRoll{0.f};
};

#endif // CAMERACONTROLLER_H
