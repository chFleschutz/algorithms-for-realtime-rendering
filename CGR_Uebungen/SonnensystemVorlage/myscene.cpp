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
    float lMoonSpeed = 7.0, lSunSpeed = lMoonSpeed / 12.0, lEarthSpeed = lMoonSpeed * 28.0;

    //Achtung: Beim Aufruf get unterscheiden:
    //Mit "&" für Referenz ist die erhaltene Component änderbar, sonst nicht!
    auto &lTrans = ECS.get<Transformation>(lSun);

    //Sonnensystem mit KB-Trafo versehen
    auto lKeyTrans = addController<KeyboardTransformationController>(lSun);
    lKeyTrans->setRotKeys(KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey, 'r', 'R',
                             KeyboardTransformationController::NoKey, KeyboardTransformationController::NoKey);
    lKeyTrans->setRotspeed(lSunSpeed);

    auto lFilled = std::make_shared<OpenGLStates>();
    lFilled->setFilled(false);

    auto lColor = std::make_shared<Color>(1.0f, 1.0f, 0.0f, 1.0f);
    ECS.get<Renderable>(lSun).addProperty(lFilled);
    ECS.get<Renderable>(lSun).addProperty(lColor);
}
