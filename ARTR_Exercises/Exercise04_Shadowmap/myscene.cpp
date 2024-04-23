#include "geometry/geometryindexedbase.hpp"
#include "manager.hpp"
#include "geometry/simplecube.hpp"
#include "geometry/simplesphere.hpp"
#include "rendering/rendercontext.hpp"
#include "shadermanager.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "shadowmaptexture.hpp"
#include "rendering/color.hpp"
#include "geometry/meshobj.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;

    auto lShaderShadow = ShaderManager::getShader(
        { lPath + "shader/shadowmap.vert", lPath + "shader/shadowmap.frag"});
    //auto lShaderPoisson = ShaderManager::getShader(
    //    { lPath + "shader/poisson.vert", lPath + "shader/poisson.frag" });
    //auto lShaderPcf = ShaderManager::getShader(
    //    { lPath + "shader/pcf.vert", lPath + "shader/pcf.frag" });

    auto lWallCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(0.2f, 5.0f, 10.0f), lShaderShadow);
    auto lFloorCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(10.0f, 1.0f, 10.0f), lShaderShadow);
    auto lPanningCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(.2f, 2.0f, 10.0f), lShaderShadow);
    auto lCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(1.5f, 1.5f, 1.5f), lShaderShadow);
    auto lSphere = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(2.0f), lShaderShadow);
    //Die Lichtquelle braucht eigentlich keine Geometry, aber so sehen wir ihren Ort
    auto lLight = addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(0.3f), lShaderShadow);

    //Füge Fahrrad der Szene hinzu
    auto lMeshOBJ = MeshOBJ(lPath + QString("models/Fahrrad.obj"));
    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShaderShadow);

    ECS.get<Transformation>(lEnt).translate(-1.5f, -2.6f, -7.f);
    ECS.get<Transformation>(lWallCube).translate(-4.9f, 0.0f, -7.0f);
    ECS.get<Transformation>(lFloorCube).translate(0.0f, -3.0f, -7.0f);
    ECS.get<Transformation>(lPanningCube).translate(0.0f, -1.5f, -7.0f);
    ECS.get<Transformation>(lSphere).translate(3.0f, 1.0f, -7.0f);
    ECS.get<Transformation>(lCube).translate(0.0f, 3.5f, -7.0f);
    ECS.get<Transformation>(lLight).translate(-5.0f, 7.0f, -7.0f);

    auto lKeyTrans = addController<KeyboardTransformationController>(lLight);
    lKeyTrans->setRotKeys('7', '/', '8', '(', '9', ')');
    lKeyTrans->setTransKeysUpper('u', 'i', 'o');

    auto lColorBicycle = std::make_shared<Color>(0.7f, 0.7f, 0.7f, 1.0f);
    ECS.get<Renderable>(lEnt).addProperty(lColorBicycle);

    auto lColorWallCube = std::make_shared<Color>(0.5f, 0.5f, 1.0f, 1.0f);
    ECS.get<Renderable>(lWallCube).addProperty(lColorWallCube);
    auto lColorFloorCube = std::make_shared<Color>(0.5f, 0.5f, 1.0f, 1.0f);
    ECS.get<Renderable>(lFloorCube).addProperty(lColorFloorCube);
    auto lColorPanningCube = std::make_shared<Color>(0.8f, 0.8f, 0.65f, 1.0f);
    ECS.get<Renderable>(lPanningCube).addProperty(lColorPanningCube);
    auto lColorSphere = std::make_shared<Color>(0.4f, 0.9f, 0.7f, 1.0f);
    ECS.get<Renderable>(lSphere).addProperty(lColorSphere);
    auto lColorCube = std::make_shared<Color>(0.8f, 0.4f, 0.65f, 1.0f);
    ECS.get<Renderable>(lCube).addProperty(lColorCube);
    auto lColorLight = std::make_shared<Color>(1.0f, 1.0f, 0.8f, 1.0f);
    ECS.get<Renderable>(lLight).addProperty(lColorLight);





    //lMat berechnet eine Offset-Transformation für die Shadowmap bzw. deren Kamera
    //Die Kamera bekommt ohnehin die Viewmatrix des Owner-Renderables
    //Die folgende Berechnung des Offsets sorgt dafür, dass die Kamera der Shadowmap
    //in X-Richtung des Renderables und 45 Grad nach unten schaut
    QMatrix4x4 lMat;
    lMat.rotate(90.f, .0f, 1.0f, 0.f);
    lMat.rotate(45.f, .0f, .0f, 1.f);
    auto lEntShadowMap = std::make_shared<ShadowMapTexture>(lLight, DepthOnly, 4, FollowEntity, lMat);
    ECS.get<Renderable>(lLight).addProperty(lEntShadowMap);
    ECS.get<Renderable>(lCube).addProperty(lEntShadowMap);
    ECS.get<Renderable>(lFloorCube).addProperty(lEntShadowMap);
    ECS.get<Renderable>(lPanningCube).addProperty(lEntShadowMap);
    ECS.get<Renderable>(lSphere).addProperty(lEntShadowMap);
    ECS.get<Renderable>(lWallCube).addProperty(lEntShadowMap); 
    ECS.get<Renderable>(lEnt).addProperty(lEntShadowMap);
}

