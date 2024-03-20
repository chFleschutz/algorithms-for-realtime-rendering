#include "transformation/keyboardtransformationcontroller.hpp"
#include "manager.hpp"
#include "geometry/geometrybase.hpp"
#include "heightmap.hpp"


void Manager::initialize()
{
    auto lHeightmap = addRenderable<GeometryBase, Heightmap>(Heightmap());
    auto lKeyTrans = addController<KeyboardTransformationController>(lHeightmap);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
}
