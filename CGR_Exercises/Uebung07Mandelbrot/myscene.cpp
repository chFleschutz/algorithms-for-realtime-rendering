#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "shadermanager.hpp"
#include "mandelbrotproperty.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    auto lShader = ShaderManager::getShader(
        QList<QString>({lPath + QString("shader/texture.vert"), lPath + QString("shader/texture.frag")}));
    auto lEnt = addRenderable<GeometryBase, SimplePlane>(SimplePlane(15.0f), lShader);
    ECS.get<Renderable>(lEnt).addProperty(std::make_shared<MandelbrotProperty>(512,512));
}
