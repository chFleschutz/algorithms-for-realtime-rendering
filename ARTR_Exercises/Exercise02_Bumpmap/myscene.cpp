#include "geometry/simpleplane.hpp"
#include "manager.hpp"
#include "ecs.hpp"
#include "rendering/material.hpp"
#include "rendering/openglstates.hpp"
#include "rendering/pointlight.hpp"
#include "shadermanager.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/meshobj.hpp"
#include "rendering/texture.hpp"
#include "rendering/renderable.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "rendering/rendercontext.hpp"
#include "transformation/trackball.hpp"


void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);
    QString lTOL_MODEL_PATH(lSORSPATH + "SORSLib/thirdparty/tinyobjloader/models/");

    auto lRenderContextEnt = RenderContext::addRenderContext();
    auto lRenderContext = RenderContext::getActiveRenderContext();
    lRenderContext.getCameraController()->setMoveSpeed(1.f);
    lRenderContext.getCameraController()->setRotSpeed(1.f);

    auto textureShader = ShaderManager::getShader({lPath + "shader/texture.vert", lPath + "shader/texture.frag"});
    auto bumpShader = ShaderManager::getShader({lPath + "shader/phong_bump_fl.vert", lPath + "shader/phong_bump_fl.frag"});

    auto lMeshOBJ = MeshOBJ(lPath+QString("models/skyship.obj"));
    lMeshOBJ.setGenerateFaceNormals(true);

    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, bumpShader);

    auto lKeyTrans = addController<KeyboardTransformationController>(lEnt);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');

    auto lTexture = std::make_shared<Texture>(lPath + QString("models/skyship_diffuse.png"));
    auto &lRenderable = ECS.get<Renderable>(lEnt);
    lRenderable.addProperty(lTexture);
    auto lBumpTexture = std::make_shared<Texture>(lPath + QString("models/skyship_bumpmap.jpg"), 1);
    lBumpTexture->setUnit(1);
    lBumpTexture->setNameString("bumpMap");
    lRenderable.addProperty(lBumpTexture);

    auto lEntOther = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
    auto &lRenderable1 = ECS.get<Renderable>(lEntOther);
//    auto lTexture1 = std::make_shared<Texture>(lPath + QString("models/masonry-wall-texture.jpg"));
    auto lTexture1 = std::make_shared<Texture>(lPath + QString("models/stone-wall-texture.jpg"));
    lRenderable1.addProperty(lTexture1);
//    auto lBumpTexture1 = std::make_shared<Texture>(lPath + QString("models/masonry-wall-normal-map.jpg"), 1);
    auto lBumpTexture1 = std::make_shared<Texture>(lPath + QString("models/stone-wall-bumpmap.jpg"), 1);
    lBumpTexture1->setUnit(1);
    lBumpTexture1->setNameString("bumpMap");
    lRenderable1.addProperty(lBumpTexture1);

    auto& lTrans1 = addTransformationEntity();
    lKeyTrans = addController<KeyboardTransformationController>(entt::to_entity(ECS, lTrans1));
    lKeyTrans->setTransKeysUpper('b', 'n', 'm');
    ECS.get<Transformation>(lEntOther).translate(0,0,-5);

    auto lMat = std::make_shared<Material>();
    lMat->setShininess(8.0f);
    lMat->setSpecular(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));
    auto lLight = std::make_shared<PointLight>();
    lLight->setViewPosition(QVector3D(-5.0f,1.f,10.0f));

    ECS.get<Renderable>(lEnt).addProperty(lMat);
    ECS.get<Renderable>(lEnt).addProperty(lLight);
    ECS.get<Renderable>(lEntOther).addProperty(lMat);
    ECS.get<Renderable>(lEntOther).addProperty(lLight);
    auto lTrackball = addController<Trackball>(lEnt);
    auto lTrackball1 = addController<Trackball>(lEntOther);
}

