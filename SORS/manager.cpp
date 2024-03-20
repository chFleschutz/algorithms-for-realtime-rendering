#include <QList>

#include "manager.hpp"
#include "transformation/transformation.hpp"
#include "geometry/geometryshared.hpp"

Transformation& Manager::addTransformationEntity()
{
    auto lEntity = ECS.create();
    return ECS.emplace<Transformation>(lEntity);
}

entt::entity Manager::addRenderableBase(std::shared_ptr<Shader> pShader)
{
    QMatrix4x4 lMat;

    auto lEntity = ECS.create();
    ECS.emplace<Renderable>(lEntity);
    ECS.emplace<Transformation>(lEntity, lMat);
    if (pShader != nullptr)
        ECS.emplace<Shader>(lEntity, *pShader);
    return lEntity;
}


entt::entity Manager::addRenderableSharedGeometry(entt::entity pEntWithSharedGeometry, std::shared_ptr<Shader> pShader)
{
    auto lEntity = addRenderableBase(pShader);
    ECS.emplace_or_replace<SharedGeometry>(lEntity, pEntWithSharedGeometry);
    return lEntity;
}
