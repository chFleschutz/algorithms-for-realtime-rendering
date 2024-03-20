/*
 *  camera.h
 *  robot
 *
 *  Created by bdr on 13.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

//! Realisierung einer einfachen  virtuellen Kamera.
//!
//! Dies ist die Basisklasse und stellt die Kameradaten
//! sowie sinnvolle Methoden für die Manipulation zur Verfügung.
//! Diese Klasse ist einfach gehalten, erlaubt die Rotation um alle Hauptachsen. Daher ist hier aber auch (theoretisch) ein Gimbal
//! Lock möglich.
class Camera
{
public:
    Camera();
    virtual ~Camera() {}

    QVector3D getPosition() const;
    void setPosition(const QVector3D& position);
    void setPosition(float x, float y, float z);

    void setRotation(float yaw, float pitch, float roll);
    void resetRotation();
    void lookAt(const QVector3D& targetPosition, const QVector3D& upDir = QVector3D(0.f, 1.f, 0.f));

    float getNearPlane() const;
    void setNearPlane(float nearPlane);
    float getFarPlane() const;
    void setFarPlane(float farPlane);
    float getDepth() const;
    float getAperture() const;
    void setAperture(float aperture);

    QVector3D getViewDir() const;
    QVector3D getUpDir() const;
    QVector3D getRightDir() const;

    //!Berechnung und Rückgabe der ViewMatrix
    QMatrix4x4 getViewMatrix() const {return mViewMatrix;}

    //!Rückgabe der Projektionsmatrix
    QMatrix4x4 getProjectionMatrix() const {return mProjectionMatrix;}

    bool isViewMatrixCalcNeeded() const;
    bool isProjectionMatrixCalcNeeded() const;
    void setNoMatrixCalcNeeded();

    bool isZRotationLocked() const;
    void lockZRotation();
    void unlockZRotation();

    void update();

    void setViewMatrix(const QMatrix4x4 &pViewMatrix);
    float getAspect() const;
    void setAspect(float pAspect);

    void setProjectionMatrix(const QMatrix4x4 &pProjectionMatrix);

protected:
    QVector3D mPosition{0.f, 0.f, 20.f};
    float mNearPlane{1.f};
    float mFarPlane{1000.f};
    float mAperture{40.f};
    float mAspect{1.0f};
    QVector3D mCCS_X, mCCS_Y, mCCS_Z;
    QMatrix4x4 mProjectionMatrix;
    QMatrix4x4 mViewMatrix;
    bool mViewMatrixCalcNeeded{true};
    bool mProjectionMatrixCalcNeeded{true};
    bool mLockZRotation{false};

    static const QVector3D cWCS_X, cWCS_Y, cWCS_Z; // Weltkoordinatenachsen

    void orthogonalizeAxes();
};

#endif // CAMERA_H
