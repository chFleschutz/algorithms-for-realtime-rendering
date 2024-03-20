#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "shadermanager.hpp"
#include "rendering/timeruniform.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/hello_glsl.vert"), lPath + QString("shader/hello_glsl.frag")}));
    auto lShaderWaving = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/wavemotion.vert"), lPath + QString("shader/hello_glsl.frag")}));
    auto lEnt1 = addRenderable<GeometryBase, SimplePlane>(SimplePlane(3.0f), lShader);
    ECS.get<Transformation>(lEnt1).translate(-3.0f, .0f,.0f);
    lEnt1 = addRenderable<GeometryBase, SimplePlane>(SimplePlane(3.0f, 3.0f, 100), lShaderWaving);
    ECS.get<Transformation>(lEnt1).translate(3.0f, .0f,.0f);
    ECS.get<Renderable>(lEnt1).addProperty(std::make_shared<TimerUniform>(2000));
}
