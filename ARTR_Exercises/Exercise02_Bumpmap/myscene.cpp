#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "rendering/material.hpp"
#include "rendering/openglstates.hpp"
#include "rendering/pointlight.hpp"
#include "shadermanager.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/meshobj.hpp"
#include "rendering/texture.hpp"
#include "rendering/renderable.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/trackball.hpp"

void Manager::initialize()
{
	QString lPath(SRCDIR); //aus common.cmake !
	QString lSORSPATH(SORSDIR);
	QString lTOL_MODEL_PATH(lSORSPATH + "SORSLib/thirdparty/tinyobjloader/models/");

	auto textureShader = ShaderManager::getShader({ lPath + "shader/texture.vert", lPath + "shader/texture.frag" });
	auto bumpShader = ShaderManager::getShader({ lPath + "shader/phong_bump_fl.vert", lPath + "shader/phong_bump_fl.frag" });

	//auto stoneTexture = std::make_shared<Texture>(lPath + QString("models/masonry-wall-texture.jpg"));
	auto stoneTexture = std::make_shared<Texture>(lPath + QString("models/stone-wall-texture.jpg"));
	//auto stoneBump = std::make_shared<Texture>(lPath + QString("models/masonry-wall-normal-map.jpg"), 1);
	//auto stoneBump = std::make_shared<Texture>(lPath + QString("models/stone-wall-bumpmap.jpg"), 1);

	auto stoneBump = std::make_shared<Texture>(lPath + QString("models/leglesslegolegolas_normal.jpg"), 1);

	auto skyShipBump = std::make_shared<Texture>(lPath + QString("models/skyship_bumpmap.jpg"), 1);
	auto skyShipDiffuse = std::make_shared<Texture>(lPath + QString("models/skyship_diffuse.png"));

	auto lMeshOBJ = MeshOBJ(lPath + QString("models/skyship.obj"));
	lMeshOBJ.setGenerateFaceNormals(true);

	auto skyShip = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, bumpShader);

	auto& lRenderable = ECS.get<Renderable>(skyShip);
	skyShipBump->setUnit(1);
	skyShipBump->setNameString("bumpMap");
	lRenderable.addProperty(skyShipBump);
	lRenderable.addProperty(skyShipDiffuse);

	auto wall = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
	auto& lRenderable1 = ECS.get<Renderable>(wall);
	ECS.get<Transformation>(wall).translate(0.0f, 0.0f, -5.0f);
	stoneBump->setUnit(1);
	stoneBump->setNameString("bumpMap");
	lRenderable1.addProperty(stoneBump);
	lRenderable1.addProperty(stoneTexture);

	auto lMat = std::make_shared<Material>();
	lMat->setShininess(8.0f);
	lMat->setSpecular(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));
	auto lLight = std::make_shared<PointLight>();
	lLight->setViewPosition(QVector3D(-5.0f, 1.f, 10.0f));

	ECS.get<Renderable>(skyShip).addProperty(lMat);
	ECS.get<Renderable>(skyShip).addProperty(lLight);
	ECS.get<Renderable>(wall).addProperty(lMat);
	ECS.get<Renderable>(wall).addProperty(lLight);

	auto lKeyTrans = addController<KeyboardTransformationController>(skyShip);
	lKeyTrans->setTransKeysUpper('j', 'k', 'l');
	lKeyTrans->setRotKeysUpper('u', 'i', 'o');

	auto& lTrans1 = addTransformationEntity();
	lKeyTrans = addController<KeyboardTransformationController>(entt::to_entity(ECS, lTrans1));
	lKeyTrans->setTransKeysUpper('b', 'n', 'm');

	auto lTrackball = addController<Trackball>(skyShip);
	auto lTrackball1 = addController<Trackball>(wall);
}

