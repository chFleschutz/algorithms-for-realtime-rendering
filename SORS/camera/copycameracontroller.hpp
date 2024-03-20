#ifndef COPYCAMERACONTROLLER_HPP
#define COPYCAMERACONTROLLER_HPP

#include "ecs.hpp"
#include "transformation/transformation.hpp"
#include <camera/cameracontroller.hpp>
#

//! Ein CameraController, der einfach die Viewmatrix einer zugeordneten Entity übernimmt
//! Ggf. kann eine zusätzliche Transformation als "Offset" zur View der Entity festgelegt werden (mOffset)
class CopyCameraController : public CameraController
{
public:
    CopyCameraController(Camera pCamera, entt::entity pEntity = entt::null)
        : CameraController(pCamera)
        , mEntity(pEntity)
    {}

    entt::entity getEntity() const;
    void setEntity(entt::entity pEntity);

    QMatrix4x4 getOffset() const;
    void setOffset(const QMatrix4x4 &pOffset);

protected:
    entt::entity mEntity{entt::null};
    QMatrix4x4 mOffset;

    virtual void doControl() override
    {
        QMatrix4x4 lOwnerViewMatrix;

        //Viewmatrix des Renderables holen, an dem der Controller hängt
        if (ECS.all_of<Transformation>(mEntity)) {
            auto lTrans = ECS.get<Transformation>(mEntity);
            lOwnerViewMatrix = lTrans.getWorldMatrix() * lTrans.getMatrix();
        }
        QMatrix4x4 lMat;
        //Zunächst das KS des Owners anwenden, dann übergebenen Offset
        lMat = mOffset * lOwnerViewMatrix.inverted();
        getCamera().setViewMatrix(lMat);
    };
};

inline entt::entity CopyCameraController::getEntity() const
{
    return mEntity;
}

inline void CopyCameraController::setEntity(entt::entity pEntity)
{
    mEntity = pEntity;
}

inline QMatrix4x4 CopyCameraController::getOffset() const
{
    return mOffset;
}

inline void CopyCameraController::setOffset(const QMatrix4x4 &pOffset)
{
    mOffset = pOffset;
}

#endif // COPYCAMERACONTROLLER_HPP
