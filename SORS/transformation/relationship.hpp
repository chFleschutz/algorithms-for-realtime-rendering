#ifndef RELATIONSHIP_HPP
#define RELATIONSHIP_HPP

#include <QString>
#include "thirdparty/entt/single_include/entt/entt.hpp"

//! Basisklasse der Visitors, in etwa dem entsprechenden Visitor-Pattern folgend
class RelationshipVisitor
{
public:
    virtual ~RelationshipVisitor() = default;

    //! Wird aufgerufen wenn das Entity traversiert wird, bevor die kinder des Entities traversiert werden.
    virtual void beforeTraverseChilds(entt::entity pEnt) {}

    //! Wird aufgerufen nachdem die Childs traversiert wurden
    virtual void afterTraverseChilds(entt::entity pEnt){}

protected:
    //! Visitor darf nicht direkt instanziiert werden.
    RelationshipVisitor() = default;
};

//! Bei der Verwendung eines ECS gibt es keinen Szenengraphen mehr, der Hierarchieinformationen zwischen
//! Entities enthalten könnte. Daher wird eine entsprechende Hierarchie als separates Konstrukt realisiert.
//! Diese Klasse speichert einen Baum, wobei statt Pointern eben die Schlüssel der Entities (entt::entity) verwendet werden.
class Relationship
{
public:
    Relationship();
    virtual ~Relationship();
    static void addChild(entt::entity pParentEnt, entt::entity pChildEnt, QString pParentName="Parent", QString pChildName="Child");
    void setNext(entt::entity pEnt) {next = pEnt;}
    void setPrev(entt::entity pEnt) {prev = pEnt;}
    void setParent(entt::entity pEnt) {parent = pEnt;}
    entt::entity getNext() {return(next);}
    entt::entity getPrev() {return(prev);}
    entt::entity getParent() {return(parent);}

    //! Durchlaufen der Childs
    static void traverseChilds(entt::entity pEnt, RelationshipVisitor& pVisitor);

    //! Durchlaufen der Siblings
    static void traverseSiblings(entt::entity pEnt, RelationshipVisitor& pVisitor);

    //! Hierarchie traversieren und Visitor anwenden
    static void visitHierarchy(entt::entity pEnt, RelationshipVisitor& pVisitor);

protected:
    //! Name der Relation, v.a. für Debugging
    QString mName{"Unknown"};
    entt::entity firstChildren{entt::null};
    entt::entity prev{entt::null};
    entt::entity next{entt::null};
    entt::entity parent{entt::null};
};

#endif // RELATIONSHIP_HPP
