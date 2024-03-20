#include "geometrysystems.hpp"
#include "geometrybase.hpp"

void GeometrySystems::preRender()
{
    //Für alle registrierten Geometrietypen...
    for (auto lCurrType: GeometryBase::renderableGeometryClasses())
    {
        if (ECS.storage(lCurrType) != nullptr)
            //Alle Entities durchlaufen
            for (auto lItem = ECS.storage(lCurrType)->begin(); lItem != ECS.storage(lCurrType)->end(); lItem++)
            {
                auto lEnt = ECS.storage(lCurrType)->at(lItem.index());
                auto lGeo = static_cast<GeometryBase*>(ECS.storage(lCurrType)->get(lEnt));
                if (lGeo->getImplementation()->needsInit())
                {
                    lGeo->deInitGeometry();
                    lGeo->getImplementation()->init();
                    lGeo->initializeGeometryBase();
                    lGeo->getImplementation()->setNeedsInit(false);
                }
            }
    }
}
