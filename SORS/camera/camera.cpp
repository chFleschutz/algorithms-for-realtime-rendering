#include <iostream>
#include <QMatrix4x4>

#include "camera.hpp"
#include "win_oglbase/openglwidget.hpp"

const QVector3D Camera::cWCS_X(1.0,0.0,0.0);
const QVector3D Camera::cWCS_Y(0.0,1.0,0.0);
const QVector3D Camera::cWCS_Z(0.0,0.0,1.0);

Camera::Camera()
{
    static auto lMainWindow = OpenGLWidget::getMainWindow();
    mAspect = (float)lMainWindow->width() / (float)lMainWindow->height();
    resetRotation();
}


QVector3D Camera::getPosition() const
{
    return mPosition;
}
void Camera::setPosition(const QVector3D& position)
{
    mPosition = position;
    mViewMatrixCalcNeeded = true;
}
void Camera::setPosition(float x, float y, float z)
{
    setPosition(QVector3D(x, y, z));
}

void Camera::setRotation(float yaw, float pitch, float roll)
{
    resetRotation();

    QMatrix4x4 lRot;

    //x und z um y (yaw)
    if (yaw != 0.0f)
    {
        lRot.setToIdentity();
        //Wir rotieren immer um die negativen Winkel, da wir die Koordinatenachsen rotieren
        //Diese müssen genau anders herum als die Kamera rotiert werden!
        lRot.rotate(-yaw, mCCS_Y);
        mCCS_X = lRot.map(mCCS_X);
        mCCS_Z = lRot.map(mCCS_Z);
    }

    //y und z um x (pitch)
    if (pitch != 0.0f)
    {
        lRot.setToIdentity();
        lRot.rotate(-pitch, mCCS_X);
        mCCS_Y = lRot.map(mCCS_Y);
        mCCS_Z = lRot.map(mCCS_Z);
    }

    //x und y um z (roll)
    if (roll != 0.0f)
    {
        lRot.setToIdentity();
        lRot.rotate(-roll, mCCS_Z);
        mCCS_X = lRot.map(mCCS_X);
        mCCS_Y = lRot.map(mCCS_Y);
    }

    //Durch das Ansammeln der Rotationen können Rundungsfehler und weitere Probleme entstehen
    //Um diese Effekte zu minimieren Orthogonalisieren wir die CCS-Achsen nach Rotationen
    orthogonalizeAxes();

    mViewMatrixCalcNeeded = true;
}
void Camera::resetRotation()
{
    mCCS_X = cWCS_X;
    mCCS_Y = cWCS_Y;
    mCCS_Z = cWCS_Z;

    mViewMatrixCalcNeeded = true;
}
void Camera::lookAt(const QVector3D& targetPosition, const QVector3D& upDir)
{
    mCCS_Z = -(targetPosition - mPosition); // View direction = -Z
    mCCS_Z.normalize();

    mCCS_X = QVector3D::crossProduct(upDir, mCCS_Z);
    mCCS_X.normalize();

    mCCS_Y = QVector3D::crossProduct(mCCS_Z, mCCS_X);
    mCCS_Y.normalize();

    mViewMatrixCalcNeeded = true;
}

float Camera::getNearPlane() const
{
    return mNearPlane;
}
void Camera::setNearPlane(float nearPlane)
{
    mNearPlane = nearPlane;
    mProjectionMatrixCalcNeeded = true;
}
float Camera::getFarPlane() const
{
    return mFarPlane;
}
void Camera::setFarPlane(float farPlane)
{
    mFarPlane = farPlane;
    mProjectionMatrixCalcNeeded = true;
}
float Camera::getDepth() const
{
    return (mFarPlane - mNearPlane);
}
float Camera::getAperture() const
{
    return mAperture;
}
void Camera::setAperture(float aperture)
{
    mAperture = aperture;
    mProjectionMatrixCalcNeeded = true;
}

QVector3D Camera::getViewDir() const
{
    return -mCCS_Z;
}
QVector3D Camera::getUpDir() const
{
    return mCCS_Y;
}
QVector3D Camera::getRightDir() const
{
    return mCCS_X;
}

bool Camera::isViewMatrixCalcNeeded() const
{
    return mViewMatrixCalcNeeded;
}
bool Camera::isProjectionMatrixCalcNeeded() const
{
    return mProjectionMatrixCalcNeeded;
}
void Camera::setNoMatrixCalcNeeded()
{
    mViewMatrixCalcNeeded = false;
    mProjectionMatrixCalcNeeded = false;
}

void Camera::orthogonalizeAxes()
{
    //Achsen des CCS wieder orthogonalisieren, da diese durch Rotationen/Rundungsfehler
    //die Orthogonalität verlieren können
    mCCS_Z.normalize();

    mCCS_Y = QVector3D::crossProduct(mCCS_Z, mCCS_X);
    mCCS_Y.normalize();

    mCCS_X = QVector3D::crossProduct(mCCS_Y, mCCS_Z);
    mCCS_X.normalize();

    //Ansammeln von Rundungsfehlern in z-Rotation verhindern
    if(mLockZRotation)
        mCCS_X.setY(0.0f);
}

bool Camera::isZRotationLocked() const
{
    return mLockZRotation;
}
void Camera::lockZRotation()
{
    mLockZRotation = true;
}
void Camera::unlockZRotation()
{
    mLockZRotation = false;
}

void Camera::update()
{
    if(this->isProjectionMatrixCalcNeeded())
    {
        mProjectionMatrix.perspective(this->getAperture(), mAspect, this->getNearPlane(), this->getFarPlane());
        mProjectionMatrixCalcNeeded = false;
    }
    if(this->isViewMatrixCalcNeeded())
    {
        mViewMatrix.setToIdentity();
        mViewMatrix.lookAt(this->getPosition(), this->getPosition() + this->getViewDir(), this->getUpDir());
        mViewMatrixCalcNeeded = false;
    }
}

void Camera::setViewMatrix(const QMatrix4x4 &pViewMatrix)
{
    mViewMatrix = pViewMatrix;
}

float Camera::getAspect() const
{
    return mAspect;
}

void Camera::setAspect(float pAspect)
{
    mAspect = pAspect;
}

void Camera::setProjectionMatrix(const QMatrix4x4 &pProjectionMatrix)
{
    mProjectionMatrix = pProjectionMatrix;
}
