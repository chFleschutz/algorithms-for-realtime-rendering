#include "transformationsystems.hpp"
#include "manager.hpp"
#include "rendering/renderable.hpp"
#include "transformation/transformation.hpp"
#include "relationship.hpp"
#include "geometry/geometrybase.hpp"

TransformationSystems::TransformationSystems() {}

void TransformationSystems::preRender() {
    TransformationSystems::visitHierarchy();
}

void TransformationSystems::visitHierarchy()
{
    auto lVisitor = TransformationHierarchyVisitor();

    //Transformationen, die hierarchisch angeordnet sind aktualisieren
    for (auto &&[lEntity, lTransformation, lRelationship]: ECS.view<Transformation, Relationship>().each()) {
        if (lRelationship.getParent() == entt::null) {
            lVisitor.resetMatrix();
            Relationship::visitHierarchy(lEntity, lVisitor);
        }
    }
}

void TransformationHierarchyVisitor::beforeTraverseChilds(entt::entity pEnt)
{
    if (ECS.all_of<Transformation>(pEnt)) {
        auto &lTrans = ECS.get<Transformation>(pEnt);
        lTrans.setWorldMatrix(mMatrix);
        mMatrix = mMatrix * lTrans.getMatrix();
    }
}

void TransformationHierarchyVisitor::afterTraverseChilds(entt::entity pEnt)
{
    if (ECS.all_of<Transformation>(pEnt)) {
        auto &lTrans = ECS.get<Transformation>(pEnt);
        mMatrix = lTrans.getWorldMatrix();
    }
}
