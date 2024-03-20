#include "manager.hpp"
#include "ecs.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "geometry/simplecircle.hpp"

void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);

    auto lCircle = addRenderable<GeometryBase, SimpleCircle>(SimpleCircle(5.0f, 16.0f, true));

    auto lKeyTrans = addController<KeyboardTransformationController>(lCircle);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
}
