#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "shadermanager.hpp"
#include "rendering/texture.hpp"
#include "rendering/timeruniform.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader_vorlagen/texture.vert"), lPath + QString("shader_vorlagen/texture_filter.frag")}));
    auto lEnt1 = addRenderable<GeometryBase, SimplePlane>(SimplePlane(3.0f), lShader);

    auto lTexture = std::make_shared<Texture>(lPath + QString("data/logo.bmp"));
    //Einkommentieren, um Bild aus dem Übungsblatt zu bekommen
    //lTexture->makeStripes(256, 10);
    ECS.get<Renderable>(lEnt1).addProperty(lTexture);
    auto lTimer = std::make_shared<TimerUniform>(5000);
    ECS.get<Renderable>(lEnt1).addProperty(lTimer);
}
