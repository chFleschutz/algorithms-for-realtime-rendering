#include "geometry/geometryindexedbase.hpp"
#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "rendering/rendertotexture.hpp"
#include "shadermanager.hpp"
#include "rendering/texture.hpp"
#include "vbuffer.hpp"
#include "geometry/simplesphere.hpp"
#include "rendering/material.hpp"
#include "rendering/color.hpp"
#include "geometry/simplecube.hpp"
#include "deferredLights.hpp"

void Manager::initialize()
{
	QString lPath = SRCDIR;

	auto lShaderGBuffer = ShaderManager::getShader({
		lPath + QString("shader/gbuffer.vert"),
		lPath + QString("shader/gbuffer.frag") });
	auto lShaderTexture = ShaderManager::getShader({
		lPath + QString("shader/texture.vert"),
		lPath + QString("shader/texture.frag") });
	auto lShaderBasic = ShaderManager::getShader({
		lPath + QString("shader/basic.vert"),
		lPath + QString("shader/basic.frag") });

	QVector<TexRenderDestFormat> lTexImagesGBuffer{
		{GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0},
		{GL_RGB32F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1},
		{GL_RGB, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT2} };

	// Random kugel an der der SceneBuffer hängt
	auto lEnt = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(0.0001f), lShaderGBuffer);
	auto lSceneBuffer = std::make_shared<RenderToTexture>(lEnt, Color2D, 0, FollowMainCam, QMatrix4x4(), lTexImagesGBuffer, PrePass, true);

	// 1. Pass -> Geometry/Kugeln erzeugen.
	std::vector<entt::entity> lKugeln;
	std::vector<PointLight> lights;
	int iMax = 5, jMax = 5;
	auto lTexture = std::make_shared<Texture>(lPath + QString("models/StoneTexture.jpg"), 4);

	QVector<QVector4D> colors;
	colors.append(QVector4D(157.f / 255.f, 13.f / 255.f, 164.f / 255.f, 1));
	colors.append(QVector4D(50.f / 255.f, 173.f / 255.f, 101.f / 255.f, 1));
	colors.append(QVector4D(150.f / 255.f, 90.f / 255.f, 221.f / 255.f, 1));
	colors.append(QVector4D(3.f / 255.f, 193.f / 255.f, 207.f / 255.f, 1));

	// Schleife zum Erstellen der Kugeln, Cubes und Lichter
	for (int i = 0; i < iMax; i++)
	{
		for (int j = 0; j < jMax; j++)
		{
			// Spheres erstellen
			auto lSphere = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(1.0f), lShaderGBuffer);
			auto& lRenderableSphere = ECS.get<Renderable>(lSphere);
			ECS.get<Transformation>(lSphere).translate(i * 5, 0, -j * 5);
			lKugeln.push_back(lSphere);

			// Eine der 4 Farben für Licht und Cubes nehmen
			QVector4D currentColor = colors[j % colors.size()];
			auto lColor = std::make_shared<Color>(currentColor);

			// Cubes zum darstellen der Lichter in der Farbe der Lichter.
			// Cubes empfangen kein Licht
			auto lCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(0.5f), lShaderBasic);
			auto& lRenderableCube = ECS.get<Renderable>(lCube);
			lRenderableCube.addProperty(lColor);

			// Position für das Licht und die Cubes
			float xOffset = i * 5 + 2.5; // Verwende Sinus für die x-Position
			float yOffset = cos(j + i); // Verwende Kosinus für die y-Position
			float zOffset = -j * 5; // Konstante z-Position
			ECS.get<Transformation>(lCube).translate(xOffset, yOffset, zOffset);

			PointLight light;
			light.setViewPosition(QVector3D(xOffset, yOffset, zOffset));
			light.setDiffuse(currentColor.toVector3D());
			light.setAttenuations(QVector2D(0.5f, 0.05f));

			lights.push_back(light);
			lRenderableSphere.setSingleRenderContext(lSceneBuffer->getContext());
			lRenderableSphere.addProperty(lTexture);

		}
	}

	// 2. Pass.
	auto lShaderDeferred = ShaderManager::getShader({ 
		lPath + QString("shader/deferred.vert"), 
		lPath + QString("shader/deferred.frag") });

	// Plane erstellen auf der das Finale Bild angezeigt wird        
	auto lEntMain = addRenderable<GeometryBase, SimplePlane>(SimplePlane(2.f), lShaderDeferred);

	// Renderable Property erstellen für die Plane. Property kümmert sich darum das die Lichter an den Shader gereicht werden
	auto lDeferredLights = std::make_shared<DeferredLights>(lEntMain);
	lDeferredLights->SetLights(lights);
	ECS.get<Renderable>(lEntMain).addProperty(lDeferredLights);
	ECS.get<Renderable>(lEntMain).setSingleRenderContext(GameSystem::getMainRenderContext());
	ECS.get<Renderable>(lEntMain).addProperty(lSceneBuffer);
}

