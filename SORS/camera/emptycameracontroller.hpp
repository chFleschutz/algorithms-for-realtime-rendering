#ifndef EMPTYCAMERACONTROLLER_HPP
#define EMPTYCAMERACONTROLLER_HPP

#include <camera/cameracontroller.hpp>

class EmptyCameraController : public CameraController
{
public:
    EmptyCameraController(Camera pCamera): CameraController(pCamera) {}

protected:
    virtual void doControl() override {};
};

#endif // EMPTYCAMERACONTROLLER_HPP
