#ifndef RENDERINGSYSTEMS_HPP
#define RENDERINGSYSTEMS_HPP

#include "rendering/rendercontext.hpp"
#include <system.hpp>

class RenderingSystems : public System
{
public:
    RenderingSystems() = delete;
    //! preRender bereitet alle Entities im Modul Rendering auf den Mainrender-Durchlauf vor. Insbesondere
    //! werden auch eigene Renderpasses vor dem Mainpass (offscreen rendering) hier aufgerufen.
    static void preRender();
    static void postRender();
    //! deactivateAll deaktiviert alle Renderables. Nach einem GBuffer Prepass kann das sinnvoll sein.
    static void deactivateAll();
protected:
    static void doFilteredAndSortedRender(RenderType pType);
};

#endif // RENDERINGSYSTEMS_HPP
