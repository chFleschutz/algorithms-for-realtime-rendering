#ifndef MANAGER_H
#define MANAGER_H

#include "win_oglbase/openglwidget.hpp"
#include "transformation/movecontroller.hpp"
#include "transformation/transformation.hpp"
#include "geometry/geometrybase.hpp"
#include "rendering/renderable.hpp"

class IGeometryImplementation;
class Shader;
class MouseKeyboardCameraController;
class Camera;

//! Enthält alle Methoden, in denen übergeordnete/unabhängige Entities/Objekte angelegt werden. Hierbei ist zu bedenken,
//! dass bei der konsequenten Verwendung von smart pointern Objekte, die dynamisch via smart pointern angelegt werden,
//! während der Konstruktion "this" nicht an andere Objekte weitergeben können. Grund hierfür ist, dass das Objekt ggf.
//! in einem nicht vollständig initialisierten Zustand von anderen verwendet wird, was zu undefiniertem Verhalten führen kann.
//! So könnte also ein Renderable in seinem eigenen Konstruktor sich nicht selbst in das ECS eintragen, sondern die Instanz,
//! die das Renderable angelegt hat, müsste das separat tun. Da das unelegant wäre, gibt es die Manager-Klasse, die
//! diese Aufgaben gesammelt übernimmt.
class Manager
{
public:
    Manager() = delete;
    static void initialize();

    //! Fügt ein Renderable hinzu. Dabei wird Shader (ggf. Default-Shader), die Geometriebasis (siehe Template-Parameter)
    //! hinzugefügt und entsprechend mit der übergebenen Implementierung verbunden
    template <typename GeometryBaseClass, typename GeometryImplementationClass>
    static entt::entity addRenderable(const GeometryImplementationClass &lGeoImplementation, std::shared_ptr<Shader> pShader=nullptr);

    //! Fügt ein Renderable hinzu, dass die Geometrie mit einem anderen Entity teilt
    //! Wichtig: Das Entity mit der "Original"-Geometrie muss erhalten bleiben, es ist "Owner" der Geometrie
    static entt::entity addRenderableSharedGeometry(entt::entity pEntWithSharedGeometry, std::shared_ptr<Shader> pShader=nullptr);

    //! Legt eine Transformation als eigenes Entity an. Das wird verwendet, um Hierarchien (siehe Relationhip) aufzubauen.
    //! Ansonsten müssen die Transformationen der jeweiligen Entities verwendet werden.
    static Transformation &addTransformationEntity();

    //! Fügt einem Entity einen Controller hinzu, der die Lage steuern kann. Das können alle von MoveController
    //! abgeleiteten Klassen sein. Der Controller greift dann auf die Transformation des Entity zu.
    template <class TController>
    static std::shared_ptr<TController> addController(entt::entity pEnt = entt::null) {
        static_assert(std::is_base_of<MoveController, TController>::value, "Controller must be derived from MoveController");
        auto lController = std::make_shared<TController>(pEnt);
        OpenGLWidget::getInstance()->addTickObject(lController);
        return lController;
    }
private:
    //! Private Methode, die gemeinsame Statements verschiedener Varianten von addRenderable enthält
    static entt::entity addRenderableBase(std::shared_ptr<Shader> pShader);
};

//! Legt ein Renderable-Entity an. Alles, was später gezeichnet werden soll, muss diese Methode oder addRenderableSharedGeometry verwenden.
//! Dabei wird ein Entity angelegt und die zuhörige Geometrie hinzugefügt.
template <typename GeometryBaseClass, typename GeometryImplementationClass>
entt::entity Manager::addRenderable(const GeometryImplementationClass &lGeoImplementation, std::shared_ptr<Shader> pShader)
{
    auto lEntity = addRenderableBase(pShader);
    GeometryBase::emplaceToEntity<GeometryBaseClass, GeometryImplementationClass>(lEntity, lGeoImplementation);
    return lEntity;
}


#endif // MANAGER_H
