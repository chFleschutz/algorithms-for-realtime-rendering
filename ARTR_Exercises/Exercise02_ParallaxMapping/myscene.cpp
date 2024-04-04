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
	QString path(SRCDIR);

	auto bumpShader = ShaderManager::getShader({ path + "shader/phong_bump_fl.vert", path + "shader/phong_bump_fl.frag" });

	// Textures
	auto masonryTexture = std::make_shared<Texture>(path + "textures/masonry-wall-texture.jpg");
	auto masonryBump = std::make_shared<Texture>(path + "textures/masonry-wall-normal-map.jpg", 1);
	masonryBump->setUnit(1);
	masonryBump->setNameString("bumpMap");

	auto stoneTexture = std::make_shared<Texture>(path + "textures/stone-wall-texture.jpg");
	auto stoneBump = std::make_shared<Texture>(path + "textures/stone-wall-bumpmap.jpg", 1);
	stoneBump->setUnit(1);
	stoneBump->setNameString("bumpMap");

	// Material and light
	auto lMat = std::make_shared<Material>();
	lMat->setShininess(8.0f);
	lMat->setSpecular(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));
	auto lLight = std::make_shared<PointLight>();
	lLight->setViewPosition(QVector3D(-5.0f, 1.f, 10.0f));

	// Create left wall
	auto wallLeft = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
	ECS.get<Transformation>(wallLeft).translate(-7.0f, 0.0f, 0.0f);
	auto& leftRenderable = ECS.get<Renderable>(wallLeft);
	leftRenderable.addProperty(masonryTexture);
	leftRenderable.addProperty(masonryBump);
	leftRenderable.addProperty(lMat);
	leftRenderable.addProperty(lLight);

	// Create right wall
	auto wallRight = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
	ECS.get<Transformation>(wallRight).translate(7.0f, 0.0f, 0.0f);
	auto& rightRenderable = ECS.get<Renderable>(wallRight);
	rightRenderable.addProperty(stoneTexture);
	rightRenderable.addProperty(stoneBump);
	rightRenderable.addProperty(lMat);
	rightRenderable.addProperty(lLight);

	// Add trackball controllers
	addController<Trackball>(wallLeft);
	addController<Trackball>(wallRight);
}

