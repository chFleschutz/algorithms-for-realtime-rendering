#include "manager.hpp"

#include "billboard.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplesphere.hpp"
#include "rendering/texture.hpp"
#include "shadermanager.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"

void Manager::initialize()
{
	QString lPath(SRCDIR); //aus common.cmake !
	QString lSORSPATH(SORSDIR);

	auto lShader = ShaderManager::getShader({
		lPath + QString("shader/basicgeoNormals.vert"),
		lPath + QString("shader/basicgeo.frag"),
		lPath + QString("shader/basicgeoNormals.geom") });

	lShader->enableShader();
	lShader->shaderProgram()->setUniformValue("startColor", 0.0, 0.0, 1.0, 1.0);
	lShader->shaderProgram()->setUniformValue("endColor", 1.0, 0.0, 0.0, 1.0);
	auto lSphere = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(), lShader);

	auto lKeyTrans = addController<KeyboardTransformationController>(lSphere);
	lKeyTrans->setTransKeysUpper('j', 'k', 'l');
	lKeyTrans->setRotKeysUpper('u', 'i', 'o');

	//Billboards (tree.png) 
	float lZ = -10.0f;
	QList<QVector4D> lPoints;
	lPoints.append(QVector4D(10.0f, 2.0f, lZ, 1.0f));
	lPoints.append(QVector4D(0.0f, 2.0f, lZ, 1.0f));
	lPoints.append(QVector4D(5.0f, 2.0f, lZ, 1.0f));
	lPoints.append(QVector4D(-10.0f, 2.0f, lZ, 1.0f));
	lPoints.append(QVector4D(20.0f, 2.0f, lZ, 1.0f));
	lPoints.append(QVector4D(-12.0f, 2.0f, lZ, 1.0f));

	auto lShaderBillboard = ShaderManager::getShader({
		lPath + QString("shader/billboard.vert"),
		lPath + QString("shader/texture.frag"),
		lPath + QString("shader/billboard.geom") });

	auto lBillboards = addRenderable<GeometryBase, Billboard>(Billboard(lPoints), lShaderBillboard);
	auto lBillboardTexture = std::make_shared<Texture>(lPath + "/tree.png");
	ECS.get<Renderable>(lBillboards).addProperty(lBillboardTexture);
	ECS.get<Renderable>(lBillboards).setIsTransparent(true);

	//Billboard Impostors 
	lZ = -1.0f;
	lPoints.clear();
	lPoints.append(QVector4D(10.0f, 0.0f, lZ, 1.0f));
	lPoints.append(QVector4D(0.0f, 0.0f, lZ, 1.0f));
	lPoints.append(QVector4D(5.0f, 0.0f, lZ, 1.0f));
	lPoints.append(QVector4D(-10.0f, 0.0f, lZ, 1.0f));
	lPoints.append(QVector4D(20.0f, 0.0f, lZ, 1.0f));
	lPoints.append(QVector4D(-12.0f, 0.0f, lZ, 1.0f));

	auto lShaderBillboardImp = ShaderManager::getShader({ 
		lPath + QString("shader/billboard.vert"), 
		lPath + QString("shader/texture.frag"), 
		lPath + QString("shader/billboard_impostor.geom") });

	auto lBillboardsImp = addRenderable<GeometryBase, Billboard>(Billboard(lPoints), lShaderBillboardImp);
	auto lBillboardTextureImp = std::make_shared<Texture>(lPath + "suzanne_impostor.png");
	//auto lBillboardTextureImp = std::make_shared<Texture>(lPath + "TestTextureAtlas3x3.png"); 
	ECS.get<Renderable>(lBillboardsImp).addProperty(lBillboardTextureImp);
	ECS.get<Renderable>(lBillboardsImp).setIsTransparent(true);

}
