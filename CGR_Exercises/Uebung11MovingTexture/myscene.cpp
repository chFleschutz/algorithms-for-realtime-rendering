#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "shadermanager.hpp"
#include "rendering/timeruniform.hpp"
#include "rendering/texture.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/texture.vert"), lPath + QString("shader/movingtexture.frag")}));
    auto lEnt = addRenderable<GeometryBase, SimplePlane>(SimplePlane(5.0f,30.0f), lShader);
    ECS.get<Transformation>(lEnt).rotate(-80.0f, QVector3D(1.0f,.0f,.0f));
    ECS.get<Renderable>(lEnt).addProperty(std::make_shared<TimerUniform>(2000));

    auto lTexture = std::make_shared<Texture>(lPath + QString("streifenstriche.bmp"));
    auto &lRenderable = ECS.get<Renderable>(lEnt);
    lRenderable.addProperty(lTexture);
    auto lKeyTrans = addController<KeyboardTransformationController>(lEnt);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
}
