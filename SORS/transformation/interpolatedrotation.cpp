#include "interpolatedrotation.hpp"
#include "transformation/transformation.hpp"

InterpolatedRotation::InterpolatedRotation(entt::entity pEntity): mEntity(pEntity)
{
}

void InterpolatedRotation::rotateStart(float angle, QVector3D axis)
{
    mStartMatrix.rotate(angle, axis);
    mStartQuat *= QQuaternion::fromAxisAndAngle(axis, angle);
}

void InterpolatedRotation::rotateEnd(float angle, QVector3D axis)
{
    mEndMatrix.rotate(angle, axis);
    mEndQuat *= QQuaternion::fromAxisAndAngle(axis, angle);
}

void InterpolatedRotation::doControl()
{
    Q_ASSERT(ECS.all_of<Transformation>(mEntity));
        auto &lTrans = ECS.get<Transformation>(mEntity);

    mT += mSpeed;
    if ((mT >= 1.0f) || (mT < 0.0f))
    {
        if (mBackandForth)
        {
            mSpeed = -mSpeed;
        }
        else
        {
            mT = 0.0f;
        }
    }
    lTrans.setMatrix(QMatrix4x4());
    if (mRepresentasQuat)
    {
        lTrans.rotateQuat(QQuaternion::nlerp(mStartQuat, mEndQuat, mT));
    }
    else
    {
        lTrans = (1 - mT) * mStartMatrix + mT * mEndMatrix;
    }
}
