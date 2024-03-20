#ifndef TRANSFORMATIONSYSTEMS_HPP
#define TRANSFORMATIONSYSTEMS_HPP

#include <QMatrix4x4>
#include "relationship.hpp"
#include "system.hpp"

//! Visitor-Klasse, die die Hierarchien von Relationship durchläuft, um zusammengesetzte Transformationen zu berechnen.
class TransformationHierarchyVisitor: public RelationshipVisitor
{
public:
    TransformationHierarchyVisitor(): mMatrix() {}
    virtual void beforeTraverseChilds(entt::entity pEnt) override;
    virtual void afterTraverseChilds(entt::entity pEnt) override;
    virtual void resetMatrix() {mMatrix.setToIdentity();}
protected:
    //! Enthält jeweils die Matrix, die aus der übergeordneteten Hierarchie "mitkommt"
    QMatrix4x4 mMatrix;
};

//! Systemklasse des Moduls transformation. Kümmert sich v.a. um das Berechnen von zusammengesetzten Transformationen,
//! wenn Hierarchien \sa Relationship verwendet werden.
class TransformationSystems: public System
{
public:
    TransformationSystems();
    static void visitHierarchy();
    static void preRender();
};

#endif // TRANSFORMATIONSYSTEMS_HPP
