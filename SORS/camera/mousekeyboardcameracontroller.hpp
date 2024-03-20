#ifndef MOUSEKEYBOARDCAMERACONTROLLER_H
#define MOUSEKEYBOARDCAMERACONTROLLER_H

#include "keyboardcameracontroller.hpp"

//! Realisierung einer Kamera, bei der die Blickrichtung über die Bewegung der Maus gesteuert werden kann. Beim 1.
//! Drücken der Rechten Maustaste wird der Mauszeiger ausgeblendet und die Bewegung der maus auf die Kamera übertragen.
//! Ein nochmaliges betätigen der rechten Maustaste macht den Mauszeiger wieder sichtbar und die Kamera wird nicht mehr
//! anhand der Mausbewegung rotiert. Während der Mauszeiger ausgeblendet ist wird er innerhalb des Fensters gehalten.
//! Wenn sich das Fenster im Vollbildmodus befindet, wird der Mauszeiger immer ausgeblendet und verfolgt.
class MouseKeyboardCameraController : public KeyboardCameraController
{
public:
    MouseKeyboardCameraController(Camera pCamera);

protected:
    virtual void doControl() override;

private:
    //! Gibt an, ob der mauszeiger versteckt, in die Mitte zentriert wird und zur Rotation der Kamera verwendet wird.
    //! Die Variable wird beim Drücken der Rechten Maustaste getoggelt.
    bool mFollowMouse = false;

    //! Wird verwendet, um zu bestimmen ob die rechte Maustaste zum ersten Mal gedrückt wurde oder ob sie schon zuvor
    //! gedrückt war.
    bool mRightMouseButtonPressed = false;
};

#endif // MOUSEKEYBOARDCAMERACONTROLLER_H
