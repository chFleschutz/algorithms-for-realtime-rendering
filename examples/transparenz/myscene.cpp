#include "manager.hpp"
#include "ecs.hpp"
#include "shadermanager.hpp"
#include "geometry/meshobj.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplecube.hpp"
#include "geometry/simpleplane.hpp"
#include "transformation/trackball.hpp"
#include "transformation/relationship.hpp"

void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);

    auto lShaderTransparent = ShaderManager::getShader(QList<QString>(
        {QString(lPath + "shader/basic.vert"), QString(lPath + "shader/transparent.frag")}));

    auto lObj = MeshOBJ(lPath + QString("models/monkey.obj"));
    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lObj);
    ECS.get<Transformation>(lEnt).translate(2.f, 0.f, 0.f);

    auto lOpak1 = addRenderable<GeometryBase, SimpleCube>(SimpleCube());
    ECS.get<Transformation>(lOpak1).translate(-1.f, 0.f, 5.f);
    auto lOpak2 = addRenderable<GeometryBase, SimpleCube>(SimpleCube());
    ECS.get<Transformation>(lOpak2).translate(0.f, 0.f, 2.f);

    auto lTransparent1 = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), lShaderTransparent);
    ECS.get<Renderable>(lTransparent1).setIsTransparent(true);
    ECS.get<Transformation>(lTransparent1).translate(-1.f, 0.f, 3.f);
    addController<Trackball>(lTransparent1);

    auto lTransparent2 = addRenderable<GeometryBase, SimpleCube>(SimpleCube(), lShaderTransparent);
    ECS.get<Renderable>(lTransparent2).setIsTransparent(true);

    //Passende Blend-Funktionen setzen
    glFunctions->glEnable(GL_BLEND);
    glFunctions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
