#ifndef ARM_HPP
#define ARM_HPP

#include "bodypart.hpp"
#include "manager.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"

class ArmController: public KeyboardTransformationController {

public:
    ArmController(entt::entity pEnt): KeyboardTransformationController(pEnt) {}
    //! Callback zum Auswerten des Inputs
    virtual void doControl() override {
        Q_ASSERT(ECS.all_of<Transformation>(mEntity));
        auto &lTrans = ECS.get<Transformation>(mEntity);
        lTrans.translate(.0f, mSize, .0f);
        KeyboardTransformationController::doControl();
        lTrans.translate(.0f, -mSize, .0f);
    }
    void setSize(float pSize) {mSize = pSize;}

protected:
    float mSize{0.f};
};

class Arm: public BodyPart {
public:
    Arm(float pWidth, float pHeight, float pDepth, char pChar): BodyPart(pWidth, pHeight, pDepth) {
        auto lKeyTrans = Manager::addController<ArmController>(mTrafoEnt);
        lKeyTrans->setRotKeys(pChar, std::toupper(pChar),
            KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey,
            KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey);
        lKeyTrans->setSize(mSize.y()/2);
    }
    Arm(float pWidth, float pHeight, float pDepth, float pRed, float pGreen, float pBlue, char pChar):
        Arm(pWidth, pHeight, pDepth, pChar) {
        setColor(pRed, pGreen, pBlue);
    }

};

#endif // ARM_HPP
