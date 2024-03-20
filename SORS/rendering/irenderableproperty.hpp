#ifndef RENDERABLEPROPERTY_HPP
#define RENDERABLEPROPERTY_HPP

#include "shader.hpp"

//! Interface für alle Eigenschaften, die ein Renderable haben kann. Alle Eigenschaften werden entsprechend dem Schema an der jeweiligen Stelle der
//! Renderdurchlaufs entsprechend aufgerufen.
class IRenderableProperty
{
public:
    IRenderableProperty() {}
    virtual ~IRenderableProperty() {}
    virtual void init(Shader &pShader) {}
    virtual void preRender(Shader &pShader) {};
    virtual void render(Shader &pShader) {};
    virtual void postRender(Shader &pShader) {}
};

#endif // RENDERABLEPROPERTY_HPP
