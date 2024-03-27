#include "transformation/keyboardtransformationcontroller.hpp"
#include "manager.hpp"
#include "geometry/geometrybase.hpp"
#include "heightmap.hpp"
#include "shadermanager.hpp"
#include "rendering/renderable.hpp"
#include "rendering/material.hpp"
#include "rendering/pointlight.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShaderPhongFragment = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/phong_fl.vert"), lPath + QString("shader/phong_fl.frag")}));

    auto lHeightmap = addRenderable<GeometryBase, Heightmap>(Heightmap(), lShaderPhongFragment);
    ECS.get<Transformation>(lHeightmap).rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    
    auto lKeyTrans = addController<KeyboardTransformationController>(lHeightmap);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');

    auto lMat = std::make_shared<Material>();
    lMat->setShininess(4.0f);
    lMat->setSpecular(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));

    auto lLight = std::make_shared<PointLight>();
    lLight->setDiffuse(QVector3D(1.0f, 1.0f, 1.0f));
    lLight->setAmbient(QVector3D(1.0f, 1.0f, 1.0f));
    lLight->setSpecular(QVector3D(1.0f, 1.0f, 1.0f));

    ECS.get<Renderable>(lHeightmap).addProperty(lMat);
    ECS.get<Renderable>(lHeightmap).addProperty(lLight);
}
