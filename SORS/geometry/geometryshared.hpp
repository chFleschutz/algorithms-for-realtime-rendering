#ifndef GEOMETRYSHARED_HPP
#define GEOMETRYSHARED_HPP

#include "thirdparty/entt/single_include/entt/entt.hpp"
#include "ecs.hpp"

//! Diese Klasse ist hilfreich, um für verschiedene Renderables dieselbe Geometrie zu teilen.
//! Vorsicht bzgl. ownership, die Entity, bei der die Geometrie gespeichert wurde owned die Geometrie und wird diese
//! ohne Prüfung ggf. freigeben.
//! Eine weitere erheblich Performanceverbesserung könnte mit instance rendering erreicht werden.
class SharedGeometry {
public:
    SharedGeometry(entt::entity pEnt): mEntWithGeometryToShare(pEnt) {}
    entt::entity getEntWithGeometryToShare() const {return mEntWithGeometryToShare;}
    void setEntWithGeometryToShare(entt::entity pEntWithGeometryToShare) {mEntWithGeometryToShare = pEntWithGeometryToShare;}

private:
    entt::entity mEntWithGeometryToShare;
};

#endif // GEOMETRYSHARED_HPP
