#include "manager.hpp"
#include "rendering/openglstates.hpp"
#include "shadermanager.hpp"
#include "geometry/tessplane.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "rendering/texture.hpp"

class BezierLine : public IGeometryImplementation
{
public:
	BezierLine(QList<QVector3D> pPoints)
	{
		mPoints += pPoints; //We want a deep copy
	}

	virtual void render() override
	{
		glFunctions->glPatchParameteri(GL_PATCH_VERTICES, 4);
		glFunctions->glDrawArrays(GL_PATCHES, 0, 4);
	}

	virtual void fillVertices(QVector<QVector4D>& vertices) override
	{
		vertices.clear();
		for (unsigned int i = 0; i < 4; i++)
			vertices.append(QVector4D(mPoints[i], 1.0));
	}
protected:
	QList<QVector3D> mPoints;
};


void Manager::initialize()
{
	QString lPath(SRCDIR); //aus common.cmake !
	QString lSORSPATH(SORSDIR);

	auto lShaderBezier = ShaderManager::getShader({ 
		lPath + QString("shader/bezier.vert"),
		lPath + QString("shader/basicRed.frag"), 
		QString(), 
		lPath + QString("shader/bezier.tcs"), 
		lPath + QString("shader/bezier.tes") });

	//auto lShaderTessCoverage = ShaderManager::getShader({ 
	//	lPath + QString("shader/tess.vert"), 
	//	lPath + QString("shader/basicRed.frag"), 
	//	QString(), 
	//	lPath + QString("shader/screencoverage.tcs"), 
	//	lPath + QString("shader/screencoverage.tes") });

	//auto lShaderTessCoverageWithHeight = ShaderManager::getShader({ 
	//	lPath + QString("shader/tess.vert"), 
	//	lPath + QString("shader/heightIllumination.frag"), 
	//	lPath + QString("shader/basicgeo.geom"), 
	//	lPath + QString("shader/screencoverageHeight.tcs"), 
	//	lPath + QString("shader/screencoverageHeight.tes") });

	// TODO: AUFGABE 1
	//Bézier Kontrollpunkte
	QList<QVector3D> lCPoints;
	lCPoints.append(QVector3D(0., 0., 0.));
	lCPoints.append(QVector3D(1., 1., 0.));
	lCPoints.append(QVector3D(2., 1., 0.));
	lCPoints.append(QVector3D(3., 0., 0.));

	auto lBezier = addRenderable<GeometryBase, BezierLine>(BezierLine(lCPoints), lShaderBezier);
	ECS.get<Transformation>(lBezier).translate(-6.0, .0f, .0f);

	// TODO: AUFGABE 2 (nur einkommentieren, keine Code-Änderungen notwendig)
//    auto lFilled = std::make_shared<OpenGLStates>();
//    lFilled->setFilled(false);
//
//    auto lTessPlane = addRenderable<GeometryBase, TessPlane>(TessPlane(5.0f, 50.0f), lShaderTessCoverage);
//    ECS.get<Transformation>(lTessPlane).translate(0.0,.0f,-25.0f);
//    ECS.get<Transformation>(lTessPlane).rotate(-85.0f, 1.0,.0f,.0f);
//    ECS.get<Renderable>(lTessPlane).addProperty(lFilled);


	// TODO: AUFGABE 3 (nur einkommentieren, keine Code-Änderungen notwendig)
//    auto lFilled1 = std::make_shared<OpenGLStates>();
//    // TODO kann zu testzwecken auf "false" gesetzt werden -> wireframe; Kann ggf. auch zusammen mit "shader/basicRed.frag" sinnvoll zum testen/debuggen genutzt werden.
//    lFilled1->setFilled(true);
//
//    auto lTessPlaneWithHeight = addRenderable<GeometryBase, TessPlane>(TessPlane(50.0f, 50.0f), lShaderTessCoverageWithHeight);
//    ECS.get<Transformation>(lTessPlaneWithHeight).translate(25.0,-25.0f,-100.0f);
//    ECS.get<Transformation>(lTessPlaneWithHeight).rotate(-85.0f, 1.0,.0f,.0f);
//    ECS.get<Renderable>(lTessPlaneWithHeight).addProperty(lFilled1);
//
//    auto lTexture = std::make_shared<Texture>(lPath + QString("models/noiseTexture.png")); // kann geändert werden, einfach mal ausprobieren!
//    lTexture->setNameString("textureMap");
//    ECS.get<Renderable>(lTessPlaneWithHeight).addProperty(lTexture);
//
//    auto heightTexture = std::make_shared<Texture>(lPath + QString("models/noiseTexture3.png"), 1); // kann geändert werden, einfach mal ausprobieren!
//    heightTexture->setNameString("heightMap");
//    ECS.get<Renderable>(lTessPlaneWithHeight).addProperty(heightTexture);
//
//    auto lKeyTrans = addController<KeyboardTransformationController>(lTessPlaneWithHeight);
//    lKeyTrans->setTransKeysUpper('j', 'k', 'l');
//    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
}
