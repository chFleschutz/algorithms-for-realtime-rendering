#include "arm.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/relationship.hpp"
#include "manager.hpp"
#include "ecs.hpp"
#include "bodypart.hpp"

void Manager::initialize()
{
    auto lTorso = BodyPart(4.f, 8.f, 4.f, 1.f, 0.f, 0.f);

    auto lLeftArm = Arm(1.f, 6.f, 1.f, .5f, .5f, 1.f, 'c');
    lLeftArm.setPosition(lTorso.getSize().x()/2 + lLeftArm.getSize().x()/2, 0.f, 0.f);

    //Weitere Teile einbauen
    //Transformationen um Controller erweitern

    //Separate Transformationen zum Schieben und Rotieren

    //Baum aufbauen
    Relationship::addChild(lTorso.getTrafoEnt(), lLeftArm.getTrafoEnt());
}
