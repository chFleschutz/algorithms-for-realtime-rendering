#ifndef KEYBOARDCAMERACONTROLLER_H
#define KEYBOARDCAMERACONTROLLER_H

#include "cameracontroller.hpp"

//! Eine klassische Kamerasteuerung über a, s, d, w, den Cursortasten sowie x, y, z
//! für eine
//! Drehung um die Hauptachsen erlaubt.
class KeyboardCameraController : public CameraController
{
public:
    KeyboardCameraController(Camera pCamera);

protected:
    virtual void doControl() override;
};

#endif  // KEYBOARDCAMERACONTROLLER_H
