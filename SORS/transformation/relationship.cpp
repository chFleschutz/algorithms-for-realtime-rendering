#include <QtGlobal>
#include "relationship.hpp"
#include "manager.hpp"

Relationship::Relationship()
{

}

Relationship::~Relationship()
{
    if (next != entt::null && ECS.all_of<Relationship>(next)) {
        auto lEnt = ECS.get<Relationship>(next);
        lEnt.setPrev(prev);
    }
    if (prev != entt::null  && ECS.all_of<Relationship>(prev)) {
        auto lEnt = ECS.get<Relationship>(prev);
        lEnt.setNext(next);
    }
    if (parent != entt::null && ECS.all_of<Relationship>(parent)) {
        auto lParentRel = ECS.get<Relationship>(parent);
        auto lMyEnt = entt::to_entity(ECS, this);
        if (lMyEnt == lParentRel.firstChildren && next != entt::null) {
            auto lNextRel = ECS.get<Relationship>(next);
            lParentRel.firstChildren = next;
        }
    }
}

void Relationship::addChild(entt::entity pParentEnt, entt::entity pChildEnt, QString pParentName, QString pChildName)
{
    auto &lParentRel = ECS.get_or_emplace<Relationship>(pParentEnt);
    auto &lChildRel = ECS.get_or_emplace<Relationship>(pChildEnt);

    lParentRel.mName = pParentName;
    lChildRel.mName = pChildName;

    if (lParentRel.firstChildren != entt::null) {
        auto &lOldChildRel = ECS.get_or_emplace<Relationship>(lParentRel.firstChildren);
        lOldChildRel.prev = pChildEnt;
        lChildRel.next = lParentRel.firstChildren;
    }

    lParentRel.firstChildren = pChildEnt;
    lChildRel.parent = pParentEnt;
}

void Relationship::traverseChilds(entt::entity pEnt, RelationshipVisitor& pVisitor)
{
    Q_ASSERT(ECS.all_of<Relationship>(pEnt));
    auto lRel = ECS.get<Relationship>(pEnt);

    if (lRel.firstChildren != entt::null)
        visitHierarchy(lRel.firstChildren, pVisitor);
}

void Relationship::traverseSiblings(entt::entity pEnt, RelationshipVisitor &pVisitor)
{
    Q_ASSERT(ECS.all_of<Relationship>(pEnt));
    auto lRel = ECS.get<Relationship>(pEnt);

    if (lRel.next != entt::null)
        visitHierarchy(lRel.next, pVisitor);
}

void Relationship::visitHierarchy(entt::entity pEnt, RelationshipVisitor &pVisitor)
{
    pVisitor.beforeTraverseChilds(pEnt);
    Relationship::traverseChilds(pEnt, pVisitor);
    pVisitor.afterTraverseChilds(pEnt);
    Relationship::traverseSiblings(pEnt, pVisitor);
}
