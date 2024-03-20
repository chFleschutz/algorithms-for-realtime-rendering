#ifndef OPENGLSTATES_HPP
#define OPENGLSTATES_HPP

#include "irenderableproperty.hpp"

//! Setzen von allgemeinen Zustandsattributen von OpenGL für Renderables. Einfaches
//! Beispiel filled oder wireframe.
class OpenGLStates: public IRenderableProperty
{
public:
    OpenGLStates() {}
    void preRender(Shader &pShader) override {
        if (mFilled)
            glFunctions->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glFunctions->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    }
    bool getFilled() const {return mFilled;}
    void setFilled(bool pFilled) {mFilled = pFilled;}
protected:
    bool mFilled{true};
};

#endif // OPENGLSTATES_HPP
