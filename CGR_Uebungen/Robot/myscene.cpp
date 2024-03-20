#include "arm.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/relationship.hpp"
#include "manager.hpp"
#include "ecs.hpp"
#include "bodypart.hpp"

void Manager::initialize()
{
    auto lTorso = BodyPart(4.f, 8.f, 4.f, 1.f, 0.f, 0.f);

    auto lHead = BodyPart(2.f, 1.f, 2.f, 1.f, 1.f, 0.f);
    lHead.setPosition(0.f, lTorso.getSize().y()/2 + lHead.getSize().y()/2, 0.f);

    auto lLeftArm = Arm(1.f, 6.f, 1.f, .5f, .5f, 1.f, 'c');
    lLeftArm.setPosition(lTorso.getSize().x()/2 + lLeftArm.getSize().x()/2, 0.f, 0.f);

    auto lRightArm = Arm(1.f, 6.f, 1.f, .5f, .5f, 1.f, 'v');
    lRightArm.setPosition(-(lTorso.getSize().x()/2 + lRightArm.getSize().x()/2), 0.f, 0.f);

    auto lLeftFinger = BodyPart(.2f, 1.f, 0.2f, 1.0f, 0.5f, 0.5f);
    lLeftFinger.setPosition(.0f, -lLeftArm.getSize().y()/2 - lLeftFinger.getSize().y()/2, 0.f);

    auto lRightFinger = BodyPart(.2f, 1.f, 0.2f, 1.0f, 0.5f, 0.5f);
    lRightFinger.setPosition(.0f, -lLeftArm.getSize().y()/2 - lLeftFinger.getSize().y()/2, 0.f);

    auto lLeg = BodyPart(1.5f, 2.0f, 1.5f, 0.0f, 0.0f, 0.8f);
    lLeg.setPosition(.0f, -lTorso.getSize().y()/2 - lLeg.getSize().y()/2, .0f);
    auto lFoot = BodyPart(3.0f, 0.2f, 3.0f, 0.0f, 0.8f, 0.0f);
    lFoot.setPosition(.0f, -lLeg.getSize().y()/2 - lFoot.getSize().y()/2, .0f);

    //Transformationen
    auto lKeyTrans = addController<KeyboardTransformationController>(lHead.getTrafoEnt());
    lKeyTrans->setRotKeys(KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey, 'k', 'K',
                          KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey);

    //Separate Transformationen zum Schieben und Rotieren
    auto lMoveEnt = entt::to_entity(ECS, addTransformationEntity());
    auto lMove = Manager::addController<KeyboardTransformationController>(lMoveEnt);
    lMove->setTransKeysUpper('b', 'n', 'm');
    auto lRotEnt = entt::to_entity(ECS, addTransformationEntity());
    auto lRotate = Manager::addController<KeyboardTransformationController>(lRotEnt);
    lRotate->setRotKeysUpper('u', 'i', 'o');

    //Baum aufbauen
    Relationship::addChild(lMoveEnt, lRotEnt, "Move", "Rotate");
    Relationship::addChild(lRotEnt, lTorso.getTrafoEnt(), "Rotate", "TorsoTrafoEnt");
    Relationship::addChild(lTorso.getTrafoEnt(), lHead.getTrafoEnt());
    Relationship::addChild(lTorso.getTrafoEnt(), lLeg.getTrafoEnt());
    Relationship::addChild(lLeg.getTrafoEnt(), lFoot.getTrafoEnt());
    Relationship::addChild(lTorso.getTrafoEnt(), lRightArm.getTrafoEnt());
    Relationship::addChild(lTorso.getTrafoEnt(), lLeftArm.getTrafoEnt());
    Relationship::addChild(lLeftArm.getTrafoEnt(), lLeftFinger.getTrafoEnt());
    Relationship::addChild(lRightArm.getTrafoEnt(), lRightFinger.getTrafoEnt());
}
