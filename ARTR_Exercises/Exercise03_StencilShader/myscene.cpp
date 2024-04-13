#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "geometry/meshobj.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "transformation/transformation.hpp"
#include "shadermanager.hpp"
#include "geometry/simplecirclefilled.hpp"
#include "rendering/rendertotexture.hpp"
#include "rendering/texture.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;

    //Prepass Erzeugen der Maske
    auto lEnt = addRenderable<GeometryBase, SimpleCircleFilled>(SimpleCircleFilled(5, 48));
    auto lEnt1 = addRenderable<GeometryBase, SimpleCircleFilled>(SimpleCircleFilled(5, 48));
    ECS.get<Transformation>(lEnt).translate(-4.0f, 0.0f, -10.0f);
    ECS.get<Transformation>(lEnt1).translate(4.0f, 0.0f, -10.0f);
    auto lMaskBuffer = std::make_shared<RenderToTexture>(lEnt, Color2D, 0, OwnCamera, QMatrix4x4(), QVector<TexRenderDestFormat>(), PrePass, false);
    ECS.get<Renderable>(lEnt).setSingleRenderContext(lMaskBuffer->getContext());
    ECS.get<Renderable>(lEnt1).setSingleRenderContext(lMaskBuffer->getContext());
    ECS.get<RenderContext>(lMaskBuffer->getContext()).setPriority(1);

    // Szene in zweite Textur rendern
    // Hintergrund
    auto lShaderTexture = ShaderManager::getShader({lPath + "shader/texture.vert", lPath + "shader/texture.frag"});
    auto lMeshOBJ = MeshOBJ(lPath+QString("models/cessna_tri.obj"));
    auto lEntMesh = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShaderTexture);
    ECS.get<Transformation>(lEntMesh).scale(QVector3D(2.0f, 2.0f, 2.0f));

    auto lEntBackground = addRenderable<GeometryBase, SimplePlane>(SimplePlane(100.f));
    ECS.get<Transformation>(lEntBackground).translate(-4.f,.0f,-10.0f);
    auto lSceneBuffer = std::make_shared<RenderToTexture>(lEntMesh, Color2D, 4, FollowMainCam, QMatrix4x4(), QVector<TexRenderDestFormat>(), PrePass, false);
    ECS.get<Renderable>(lEntBackground).setSingleRenderContext(lSceneBuffer->getContext());
    ECS.get<Renderable>(lEntMesh).setSingleRenderContext(lSceneBuffer->getContext());
    ECS.get<RenderContext>(lSceneBuffer->getContext()).setPriority(2);

    //Hauptrenderpass verknüpft Ergebnisse
    auto lShaderMask = ShaderManager::getShader({lPath + "shader/stencil.vert", lPath + "shader/stencil.frag"});
    auto lEntMain = addRenderable<GeometryBase, SimplePlane>(SimplePlane(2.f), lShaderMask);
    ECS.get<Renderable>(lEntMain).setSingleRenderContext(GameSystem::getMainRenderContext());

    ECS.get<Renderable>(lEntMain).addProperty(lSceneBuffer);
    ECS.get<Renderable>(lEntMain).addProperty(lMaskBuffer);
}
