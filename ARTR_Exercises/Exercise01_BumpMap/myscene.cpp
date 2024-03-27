#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "shadermanager.hpp"
#include "rendering/pointlight.hpp"
#include "rendering/material.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/trackball.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/tbn.vert"), lPath + QString("shader/tbn.frag")}));
    auto lEnt1 = addRenderable<GeometryBase, SimplePlane>(SimplePlane(15.0f), lShader);

    auto lMat = std::make_shared<Material>();
    lMat->setShininess(5.0f);
    lMat->setSpecular(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
    auto lLight = std::make_shared<PointLight>();

    ECS.get<Renderable>(lEnt1).addProperty(lMat);
    ECS.get<Renderable>(lEnt1).addProperty(lLight);
    auto lKeyTrans = addController<KeyboardTransformationController>(lEnt1);
    auto lTrackball = addController<Trackball>(lEnt1);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
}
