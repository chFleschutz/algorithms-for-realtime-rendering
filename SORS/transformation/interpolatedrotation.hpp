#ifndef INTERPOLATEDROTATION_HPP
#define INTERPOLATEDROTATION_HPP

#include <QMatrix4x4>
#include <QQuaternion>
#include "movecontroller.hpp"
#include "ecs.hpp"

class InterpolatedRotation : public MoveController
{
public:
    InterpolatedRotation(entt::entity pEntity = entt::null);
    void rotateStart(float angle, QVector3D axis);
    void rotateEnd(float angle, QVector3D axis);
    virtual void doControl() override;
    void setSpeed(float s) {mSpeed = s;}
    void setBackandForth(bool b) {mBackandForth = b;}
    void setRepresentAsQuat(bool b) {mRepresentasQuat = b;}

protected:
    entt::entity mEntity{entt::null};
    QMatrix4x4 mStartMatrix, mEndMatrix;
    QQuaternion mStartQuat, mEndQuat;
    float mT{0.0f}, mSpeed{0.01f};
    bool mBackandForth{false};
    bool mRepresentasQuat{true};
};

#endif // INTERPOLATEDROTATION_HPP
