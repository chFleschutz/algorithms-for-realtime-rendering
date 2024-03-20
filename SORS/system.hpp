#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "ecs.hpp"

//! Von dieser Klasse leiten alle systems ab, die Klasse GameSystem sorgt dafür,
//! dass die Methoden der "systems" entsprechend aufgerufen werden
class System
{
public:
    static void preRender() {}
    static void render() {}
    static void postRender() {}
};

//! Implementiert den Loop der Anwendung und wird zyklisch von OpenGLWidget::paintGL gerufen.
class GameSystem: public System
{
public:
    GameSystem() = default;
    //! Wird von openglwidget nach Initialisierung gerufen und steuert die Renderdurchläufe
    static void mainLoop();
    static void preRender();
    static void render();
    static void postRender();
    static entt::entity getMainRenderContext();

protected:
    static void doRender(bool pTransparent);
};

#endif // SYSTEMS_HPP
