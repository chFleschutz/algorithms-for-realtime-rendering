#include "transformation.hpp"

Transformation::Transformation(QMatrix4x4 pMat):mMatrix(pMat)
{
}

void Transformation::translate(float tx, float ty, float tz)
{
    mMatrix.translate(QVector3D(tx, ty, tz));
}
void Transformation::rotate(float angle, const QVector3D& axis)
{
    mMatrix.rotate(angle, axis);
}

void Transformation::rotateQuat(QQuaternion pQuat)
{
    mMatrix.rotate(pQuat);
}
void Transformation::rotate(float angle, float x, float y, float z)
{
    mMatrix.rotate(angle, x, y, z);
}
void Transformation::scale(const QVector3D& scale)
{
    mMatrix.scale(scale);
}
void Transformation::scale(float sx, float sy, float sz)
{
    mMatrix.scale(sx, sy, sz);
}
void Transformation::resetTrafo()
{
    mMatrix.setToIdentity();
}

void Transformation::setMatrix(const QMatrix4x4& modelMatrix)
{
    mMatrix = modelMatrix;
}

QMatrix4x4 Transformation::getWorldMatrix() const
{
    return mWorldMatrix;
}

void Transformation::setWorldMatrix(const QMatrix4x4 &pWorldMatrix)
{
    mWorldMatrix = pWorldMatrix;
}
