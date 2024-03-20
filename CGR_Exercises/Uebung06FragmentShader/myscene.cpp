#include "manager.hpp"
#include "geometry/simplecube.hpp"
#include "shadermanager.hpp"
#include "rotaxis.hpp"
#include "rendering/timeruniform.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/myfirstshader.vert"), lPath + QString("shader/myfirstshader.frag")}));
    auto lEnt1 = addRenderable<GeometryBase, SimpleCube>(SimpleCube(3.0f), lShader);
    ECS.get<Renderable>(lEnt1).addProperty(std::make_shared<RotAxisProperty>());
    ECS.get<Renderable>(lEnt1).addProperty(std::make_shared<TimerUniform>(10000));
}
