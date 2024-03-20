#ifndef GEOMETRYSYSTEMS_H
#define GEOMETRYSYSTEMS_H

#include "system.hpp"

//! Enthält die systems des Moduls Geometry.
class GeometrySystems: public System
{
public:
    GeometrySystems() = delete;
    //! preRender bereitet alle Geometrien gesammelt auf das Rendering vor. Das beinhaltet insbsd. ggf. notwendige Initialisierungen.
    static void preRender();
};

#endif // GEOMETRYSYSTEMS_H
