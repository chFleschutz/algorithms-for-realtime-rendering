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

	auto bumpShader = ShaderManager::getShader({ path + "shader/tbn.vert", path + "shader/parallax.frag" });

	// Textures
	auto masonryDiffuse = std::make_shared<Texture>(path + "textures/masonry-wall-diffuse.jpg");
	masonryDiffuse->setUnit(0);
	masonryDiffuse->setNameString("diffuseMap");
	auto masonryNormal = std::make_shared<Texture>(path + "textures/masonry-wall-normal.jpg");
	masonryNormal->setUnit(1);
	masonryNormal->setNameString("normalMap");
	auto masonryDepth = std::make_shared<Texture>(path + "textures/masonry-wall-depth.jpg");
	masonryDepth->setUnit(2);
	masonryDepth->setNameString("depthMap");

	auto stoneTexture = std::make_shared<Texture>(path + "textures/brick-diffuse.jpg");
	stoneTexture->setNameString("diffuseMap");
	stoneTexture->setUnit(0);
	auto stoneNormal = std::make_shared<Texture>(path + "textures/brick-normal.jpg");
	stoneNormal->setNameString("normalMap");
	stoneNormal->setUnit(1);
	auto stoneDepth = std::make_shared<Texture>(path + "textures/brick-depth.jpg");
	stoneDepth->setNameString("depthMap");
	stoneDepth->setUnit(2);

	// Material and light
	auto lMat = std::make_shared<Material>();
	lMat->setShininess(2.0f);
	lMat->setSpecular(QVector4D(0.8f, 0.8f, 0.8f, 1.0f));
	auto lLight = std::make_shared<PointLight>();
	lLight->setViewPosition(QVector3D(-10.0f, 1.0f, 10.0f));

	// Create left wall
	auto wallLeft = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
	ECS.get<Transformation>(wallLeft).translate(-7.0f, 0.0f, 0.0f);
	auto& leftRenderable = ECS.get<Renderable>(wallLeft);
	leftRenderable.addProperty(masonryDiffuse);
	leftRenderable.addProperty(masonryNormal);
	leftRenderable.addProperty(masonryDepth);
	leftRenderable.addProperty(lMat);
	leftRenderable.addProperty(lLight);

	// Create right wall
	auto wallRight = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), bumpShader);
	ECS.get<Transformation>(wallRight).translate(7.0f, 0.0f, 0.0f);
	auto& rightRenderable = ECS.get<Renderable>(wallRight);
	rightRenderable.addProperty(stoneTexture);
	rightRenderable.addProperty(stoneNormal);
	rightRenderable.addProperty(stoneDepth);
	rightRenderable.addProperty(lMat);
	rightRenderable.addProperty(lLight);

	// Add trackball controllers
	addController<Trackball>(wallLeft);
	addController<Trackball>(wallRight);
}

