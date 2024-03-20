#include "manager.hpp"
#include "ecs.hpp"
#include "rendering/openglstates.hpp"
#include "shadermanager.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/meshobj.hpp"
#include "rendering/texture.hpp"
#include "rendering/renderable.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "rendering/rendercontext.hpp"
#include "transformation/relationship.hpp"

void Manager::initialize()
{
    QString lPath(SRCDIR); //aus common.cmake !
    QString lSORSPATH(SORSDIR);
    QString lTOL_MODEL_PATH(lSORSPATH + "SORSLib/thirdparty/tinyobjloader/models/");

    auto lRenderContextEnt = RenderContext::addRenderContext();
    auto lRenderContext = RenderContext::getActiveRenderContext();
    lRenderContext.getCameraController()->setMoveSpeed(1.f);
    lRenderContext.getCameraController()->setRotSpeed(1.f);

//    auto lShader = ShaderManager::getDefaultShader();
    auto lShader =
        ShaderManager::getShader(QList<QString>({lPath + QString("shader/texture.vert"), lPath + QString("shader/texture.frag")}));
//    auto lTexture = std::make_shared<Texture>(lPath + "models/ogrehead_diffuse.png");
    auto lMeshOBJ = MeshOBJ(lPath+QString("models/mario/mario-sculpture.obj"));
//    auto lMeshOBJ = MeshOBJ(lTOL_MODEL_PATH+QString("map-bump.obj"));
//    auto lMeshOBJ = MeshOBJ(lPath+QString("models/livingroom/InteriorTest.obj"));

    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShader);
//    auto &lTrans = sRegistry.get<Transformation>(lEnt);
//    lTrans.translate(5.,0.,0.);

    auto lKeyTrans = addController<KeyboardTransformationController>(lEnt);
    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');

    auto& lTrans = ECS.get<Transformation>(lEnt);
    lTrans.translate(-50.0f, .0f, -200.0f);

    //Die Texturen hängen vom Material ab, wir laden hier einfach die Textur von Material 0
    auto lTexture = std::make_shared<Texture>(lPath + QString("models/mario/") +lMeshOBJ.getTextureDiffusePath(0));
    auto &lRenderable = ECS.get<Renderable>(lEnt);
    lRenderable.addProperty(lTexture);

    //Zweites Renderable verwendet dieselbe Geomtrie und dieselbe Textur
    auto lEntOther =addRenderableSharedGeometry(lEnt,lShader);
    auto& lRenderable1 = ECS.get<Renderable>(lEntOther);
    lRenderable1.addProperty(lTexture);

    auto& lTrans1 = addTransformationEntity();
    lKeyTrans = addController<KeyboardTransformationController>(entt::to_entity(ECS, lTrans1));
    lKeyTrans->setTransKeysUpper('b', 'n', 'm');
    ECS.get<Transformation>(lEntOther).translate(100, 0,0);
    Relationship::addChild(lEnt, entt::to_entity(ECS, lTrans1));
    Relationship::addChild(entt::to_entity(ECS, lTrans1), lEntOther);
}
