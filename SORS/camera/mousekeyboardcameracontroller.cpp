#include <QVector2D>

#include "mousekeyboardcameracontroller.hpp"
#include "input/inputregistry.hpp"
#include "win_oglbase/openglwidget.hpp"
#include "manager.hpp"

MouseKeyboardCameraController::MouseKeyboardCameraController(Camera pCamera)
    : KeyboardCameraController(pCamera), mFollowMouse(false), mRightMouseButtonPressed(false)
{
}

void MouseKeyboardCameraController::doControl()
{
    auto lCamera = ECS.get<Camera>(mCamera);

    KeyboardCameraController::doControl();
    const QVector2D mouseMoveVector = InputRegistry::getInstance().getMouseInput()->getMouseMove();

    if (!OpenGLWidget::getInstance()->geometry().contains(OpenGLWidget::getInstance()->mapFromGlobal(QCursor::pos())))
    {
        // Wenn das Fenster nicht das aktuelle Fenster ist -> Kamerasteuerung abrechen und Maus wieder sichtbar
        // machen.
        mFollowMouse = false;
        OpenGLWidget::getInstance()->setCursor(Qt::ArrowCursor);
        return;
    }

    if (InputRegistry::getInstance().getMouseInput()->isMouseButtonPressed(Qt::RightButton))
    {
        if (!mRightMouseButtonPressed)
        {
            // Rechte maustaste wurde zum ersten mal gedrückt
            mRightMouseButtonPressed = true;
            mFollowMouse = !mFollowMouse;
        }
    }
    else
    {
        mRightMouseButtonPressed = false;
    }

    // Kamerasteuerung ausführen
    if (mFollowMouse)
    {
        OpenGLWidget& window = *OpenGLWidget::getInstance();
        window.setCursor(Qt::BlankCursor); // Mauszeiger ausblenden
        QRect geometry = window.geometry();
        if (mouseMoveVector.lengthSquared() < geometry.height() * geometry.bottom() / 10.0f)
        {
            // große bewegungen ignorieren da diese meist durch das Zentrieren des Mauszeigers entstehen und
            // nicht durch bewegung der maus.
            mYaw += mouseMoveVector.x() * mRotSpeed;
            mPitch -= mouseMoveVector.y() * mRotSpeed;
            lCamera.setRotation(mYaw, mPitch, 0.f);
        }

        QPoint localCursorPos = window.mapFromGlobal(QCursor::pos());
        // verkleinere das Rechteck auf jeder seite um 50, sodass der Cursor bei schnellen Bewegungen nicht so leicht aus
        // dem Fenster hinaus glitchen kann.
        geometry.adjust(50, 50, -50, -50);
        if (!geometry.contains(localCursorPos))
        {
            // Mauszeiger in die Mitte des Fensters setzen, sodass neue Bewegungen der Maus erfast werden können.
            QCursor::setPos(window.mapToGlobal(geometry.center()));
        }
    }
    else
    {
        OpenGLWidget::getInstance()->setCursor(Qt::ArrowCursor);
    }
}
