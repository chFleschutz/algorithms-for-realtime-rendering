#include "manager.hpp"
#include "ecs.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplesphere.hpp"
#include "geometry/simplecube.hpp"
#include "transformation/transformation.hpp"
#include "shadermanager.hpp"
#include "rendering/material.hpp"
#include "rendering/pointlight.hpp"

void Manager::initialize()
{
    QString lPath = SORSDIR;
    auto lShaderLambert = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/lambert_vl.vert"), lPath + QString("shader/passthrough.frag")}));
    auto lShaderPhong = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/phong_vl.vert"), lPath + QString("shader/passthrough.frag")}));
    auto lPhongLighted = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(4.f, 10, 10), lShaderPhong);
//    auto lPhongLighted = addRenderable<GeometryBase, SimpleCube>(SimpleCube(4.f), lShaderPhong);
    auto lLambertLighted = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(4.f, 10, 10), lShaderLambert);
    auto lMat = std::make_shared<Material>();

    ECS.get<Renderable>(lLambertLighted).addProperty(std::make_shared<Material>());
    ECS.get<Renderable>(lLambertLighted).addProperty(std::make_shared<PointLight>());
    ECS.get<Renderable>(lPhongLighted).addProperty(std::make_shared<Material>());
    ECS.get<Renderable>(lPhongLighted).addProperty(std::make_shared<PointLight>());

    auto &lTrans = ECS.get<Transformation>(lPhongLighted);
    lTrans.translate(.0f, 5.0f, -10.f);
    auto &lTrans1 = ECS.get<Transformation>(lLambertLighted);
    lTrans1.translate(.0f, -5.0f, -10.f);
}
