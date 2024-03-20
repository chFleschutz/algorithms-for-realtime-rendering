#include "manager.hpp"
#include "ecs.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplesphere.hpp"
#include "geometry/simplecube.hpp"
#include "geometry/simpleplane.hpp"
#include "transformation/transformation.hpp"
#include "shadermanager.hpp"
#include "rendering/material.hpp"
#include "rendering/pointlight.hpp"

void Manager::initialize()
{
    QString lSORSPath = SORSDIR;
    QString lPath = SRCDIR;
    auto lShaderPhongFragment = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/phong_fl.vert"), lPath + QString("shader/phong_fl.frag")}));
    auto lShaderToon = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/phong_fl.vert"), lPath + QString("shader/toon.frag")}));
    auto lShaderBrick = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/brick.vert"), lPath + QString("shader/brick.frag")}));
    auto lShaderPhong = ShaderManager::getShader(
        QList<QString>({lSORSPath + QString("shader/phong_vl.vert"), lSORSPath + QString("shader/passthrough.frag")}));
    auto lPhongLighted = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(4.f, 20, 20), lShaderPhong);
    auto lLambertLighted = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(4.f, 20, 20), lShaderPhongFragment);
    auto lToonLighted = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(4.f, 20, 20), lShaderToon);
    auto lBrick = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), lShaderBrick);

    auto lMat = std::make_shared<Material>();
    lMat->setShininess(5.0f);
    lMat->setSpecular(QVector4D(0.3f, 0.3f, 0.3f, 1.0f));
    auto lLight = std::make_shared<PointLight>();

    ECS.get<Renderable>(lLambertLighted).addProperty(lMat);
    ECS.get<Renderable>(lLambertLighted).addProperty(lLight);
    ECS.get<Renderable>(lPhongLighted).addProperty(lMat);
    ECS.get<Renderable>(lPhongLighted).addProperty(lLight);
    ECS.get<Renderable>(lToonLighted).addProperty(lMat);
    ECS.get<Renderable>(lToonLighted).addProperty(lLight);

    auto &lTrans = ECS.get<Transformation>(lPhongLighted);
    lTrans.translate(.0f, 5.0f, -10.f);
    auto &lTrans1 = ECS.get<Transformation>(lLambertLighted);
    lTrans1.translate(.0f, -5.0f, -10.f);
    auto &lTrans2 = ECS.get<Transformation>(lToonLighted);
    lTrans2.translate(10.0f, 0.0f, -10.f);
    auto &lTrans3 = ECS.get<Transformation>(lBrick);
    lTrans3.translate(-10.0f, 0.0f, -10.f);
}
