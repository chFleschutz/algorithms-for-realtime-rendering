#include "manager.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/meshobj.hpp"
#include "shadermanager.hpp"
#include "rendering/timeruniform.hpp"
#include "rendering/openglstates.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/morph.vert"), lPath + QString("shader/morph.frag")}));
//    auto lMeshOBJ = MeshOBJ(lPath+QString("models/skycaveship.obj"));
    auto lMeshOBJ = MeshOBJ(lPath+QString("models/monkey.obj"));
    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShader);
    ECS.get<Renderable>(lEnt).addProperty(std::make_shared<TimerUniform>(1000));
    auto lState = std::make_shared<OpenGLStates>();
    lState->setFilled(false);
    ECS.get<Renderable>(lEnt).addProperty(lState);
}
