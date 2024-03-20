#ifndef TRACKBALL_HPP
#define TRACKBALL_HPP

#include "ecs.hpp"
#include "thirdparty/entt/single_include/entt/entt.hpp"

#include "movecontroller.hpp"
#include "transformation/transformation.hpp"
#include "input/inputregistry.hpp"
#include "win_oglbase/openglwidget.hpp"

//! Diese Klasse realisiert einen einfachen Trackball und kann als Controller zu Entities hinzugefügt werden.
//! Mit gedrückter linker Maustaste wird dann das jeweilige Entity entsprechend rotiert.
class Trackball : public MoveController
{
public:
    Trackball(entt::entity pEntity = entt::null)
        : mEntity(pEntity)
    {
        mRotSpeed = 1.0f;
    }
    virtual void doControl() override
    {
        auto lWindow = OpenGLWidget::getInstance()->getMainWindow();
        if (InputRegistry::getInstance().getMouseInput()->isMouseButtonPressed(Qt::LeftButton)) {
            if (!mLeftMouseButtonPressed) {
                // Rechte maustaste wurde "neu" gedrückt
                mLeftMouseButtonPressed = true;
                auto lMouseStartPos = InputRegistry::getInstance().getMouseInput()->getMousePosition();
                mStartPoint = QVector2D(lMouseStartPos.x(),lMouseStartPos.y());
            }
            auto &lTrans = ECS.get<Transformation>(mEntity);

            auto lMousePos = InputRegistry::getInstance().getMouseInput()->getMousePosition();
            const QVector2D mouseMoveVector = QVector2D(lMousePos.x(),lMousePos.y()) - mStartPoint;
            lWindow->setCursor(Qt::BlankCursor);
            QRect geometry = lWindow->geometry();

            //Letzte Rotation jeweils rückgängig machen und neue anwenden.
            lTrans.rotate(-mLastXRot, QVector3D(1.0f, .0f, .0f));
            lTrans.rotate(-mLastYRot, QVector3D(.0f, 1.0f, .0f));
            mLastXRot = -mouseMoveVector.y() / geometry.height() * mRotSpeed * 360.0f;
            mLastYRot = mouseMoveVector.x() / geometry.width() * mRotSpeed * 360.0f;
            lTrans.rotate(mLastYRot, QVector3D(.0f, 1.0f, .0f));
            lTrans.rotate(mLastXRot, QVector3D(1.0f, .0f, .0f));

        } else {
            mLeftMouseButtonPressed = false;
            lWindow->setCursor(Qt::ArrowCursor);
        }
    }

protected:
    entt::entity mEntity{entt::null};
    bool mLeftMouseButtonPressed = false;
    QVector2D mStartPoint{0.0f, 0.0f};

private:
    float mLastYRot{0.f}, mLastXRot{0.f};
};

#endif // TRACKBALL_HPP
