#include "keyboardcameracontroller.hpp"
#include "input/inputregistry.hpp"
#include "manager.hpp"

KeyboardCameraController::KeyboardCameraController(Camera pCamera) :
    CameraController(pCamera)
{
}

void KeyboardCameraController::doControl()
{
    QVector3D deltaPosition;
    auto &lCamera = ECS.get<Camera>(mCamera);
    auto keyIn = InputRegistry::getInstance().getKeyboardInput();
    if (keyIn->isKeyPressed('w'))
    {
        deltaPosition += lCamera.getViewDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('s'))
    {
        deltaPosition -= lCamera.getViewDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('a'))
    {
        deltaPosition -= lCamera.getRightDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('d'))
    {
        deltaPosition += lCamera.getRightDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Left))
    {
        mYaw -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Right))
    {
        mYaw += mRotSpeed;
    }
    if (keyIn->isKeyPressed('e'))
    {
        mRoll += mRotSpeed;
    }
    if (keyIn->isKeyPressed('q'))
    {
        mRoll -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Up))
    {
        mPitch -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Down))
    {
        mPitch += mRotSpeed;
    }
    if (keyIn->isKeyPressed('x'))
    {
        deltaPosition -= QVector3D(1.f, 0.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('X'))
    {
        deltaPosition += QVector3D(1.f, 0.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('y'))
    {
        deltaPosition -= QVector3D(0.f, 1.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('Y'))
    {
        deltaPosition += QVector3D(0.f, 1.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('z'))
    {
        deltaPosition -= QVector3D(0.f, 0.f, 1.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('Z'))
    {
        deltaPosition += QVector3D(0.f, 0.f, 1.f) * mMoveSpeed;
    }

    lCamera.setRotation(mYaw, mPitch, mRoll);
    lCamera.setPosition(lCamera.getPosition() + deltaPosition);
    lCamera.update();
}
