#include "manager.hpp"
#include "ecs.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplesphere.hpp"
#include "transformation/transformation.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/relationship.hpp"
#include "rendering/renderable.hpp"
#include "rendering/openglstates.hpp"
#include "rendering/color.hpp"

void Manager::initialize()
{
    auto lSun = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(5.f));
    auto lEarth = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(1.f, 15, 15));
    auto lMoon = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(.25f, 10, 10));

    float lMoonSpeed = 7.0, lSunSpeed = lMoonSpeed / 12.0, lEarthSpeed = lMoonSpeed * 28.0;

    //Achtung: Beim Aufruf get unterscheiden:
    //Mit "&" für Referenz ist die erhaltene Component änderbar, sonst nicht!
    auto &lTrans = ECS.get<Transformation>(lSun);
    //Sonnensystem neigen, damit die Rotationen besser sichtbar werden
    lTrans.translate(.0f, .0f, -10.f);
    lTrans.rotate(45.f, 1.f, 0.f, 0.f);

    auto &lEarthOrbit = addTransformationEntity();
    lEarthOrbit.translate(7.f, 0.f, 0.f);

    auto &lMoonRotation = addTransformationEntity();
    auto &lMoonOrbit = addTransformationEntity();
    lMoonOrbit.translate(2.f, 0.f, 0.f);

    Relationship::addChild(lSun, entt::to_entity(ECS, lEarthOrbit));
    Relationship::addChild(entt::to_entity(ECS, lEarthOrbit), lEarth);
    Relationship::addChild(entt::to_entity(ECS, lEarthOrbit), entt::to_entity(ECS, lMoonRotation));
    Relationship::addChild(entt::to_entity(ECS, lMoonRotation), entt::to_entity(ECS, lMoonOrbit));
    Relationship::addChild(entt::to_entity(ECS, lMoonOrbit), lMoon);

    //Sonnensystem, Erde und Mondrotation mit KB-Trafo versehen
    auto lKeyTrans = addController<KeyboardTransformationController>(lSun);
    lKeyTrans->setRotKeys(KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey, 'r', 'R',
                             KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey);
    lKeyTrans->setRotspeed(lSunSpeed);

    lKeyTrans = addController<KeyboardTransformationController>(lEarth);
    lKeyTrans->setRotKeys('r', 'R', KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey,
                          KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey);
    lKeyTrans->setRotspeed(lEarthSpeed);

    lKeyTrans = addController<KeyboardTransformationController>(entt::to_entity(ECS, lMoonRotation));
    lKeyTrans->setRotKeys(KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey,
                          KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey, 'r', 'R');
    lKeyTrans->setRotspeed(lMoonSpeed);

    auto lFilled = std::make_shared<OpenGLStates>();
    lFilled->setFilled(false);

    auto lColor = std::make_shared<Color>(1.0f, 1.0f, 0.0f, 1.0f);
    ECS.get<Renderable>(lSun).addProperty(lFilled);
    ECS.get<Renderable>(lSun).addProperty(lColor);

    lColor = std::make_shared<Color>(0.f,0.f,1.f,1.f);
    ECS.get<Renderable>(lEarth).addProperty(lColor);
    ECS.get<Renderable>(lEarth).addProperty(lFilled);

    lColor = std::make_shared<Color>(.5f,.5f,.5f,1.f);
    ECS.get<Renderable>(lMoon).addProperty(lColor);
    ECS.get<Renderable>(lMoon).addProperty(lFilled);

}
