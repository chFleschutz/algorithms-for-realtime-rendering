#include "manager.hpp"
#include "geometry/simplecube.hpp"
#include "geometry/simpleplane.hpp"
#include "rendering/rendercontext.hpp"
#include "shadermanager.hpp"
#include "rendering/texture.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "rendering/rendertotexture.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;

    //Skybox
    auto lShader = ShaderManager::getShader(QList<QString>(
        {lPath + QString("shader/texturecube.vert"), lPath + QString("shader/texturecube.frag")}));
    auto lEnt = addRenderable<GeometryBase, SimpleCube>(SimpleCube(500.0f), lShader);
    auto lTexture = std::make_shared<Texture>(lPath + QString("colosseum"));
    auto &lRenderable = ECS.get<Renderable>(lEnt);
    lRenderable.addProperty(lTexture);

    //Würfel mit statischer Environment Map
    auto lShader1 = ShaderManager::getDefaultShader();
    auto lShaderEnvMap = ShaderManager::getShader(QList<QString>(
        {lPath + QString("shader/envmap.vert"), lPath + QString("shader/envmap.frag")}));
    auto lEntEnv = addRenderable<GeometryBase, SimpleCube>(SimpleCube(5.0f), lShaderEnvMap);
    //Skybox-Textur für Reflection-Lookup hinzufügen
    ECS.get<Renderable>(lEntEnv).addProperty(lTexture);
    ECS.get<Transformation>(lEntEnv).translate(15.0f, .0f, -20.0f);
    auto lKeyTransEnv = addController<KeyboardTransformationController>(lEntEnv);
    lKeyTransEnv->setTransKeysUpper('b', 'n', 'm');


    // Fläche als "Boden"
    auto lShaderFloor = ShaderManager::getShader(QList<QString>(
        {lPath + QString("shader/texture.vert"), lPath + QString("shader/texture.frag")}));
    auto lEntFloor = addRenderable<GeometryBase, SimplePlane>(SimplePlane(20.0f, 500.0f), lShaderFloor);
    auto lTextureFloor = std::make_shared<Texture>();
    lTextureFloor->makeStripes(50, 2);
    auto &lRenderableFloor = ECS.get<Renderable>(lEntFloor);
    lRenderableFloor.addProperty(lTextureFloor);
    ECS.get<Transformation>(lEntFloor).translate(0.0f, -10.0f, 0.0f);
    ECS.get<Transformation>(lEntFloor).rotate(90.0f, 1.0f, 0.0f, 0.0f);

    auto lShaderCube = ShaderManager::getShader(QList<QString>(
        {lPath + QString("../../SORS/shader/basic.vert"),
         lPath + QString("../../SORS/shader/basicred.frag")}));
    auto lEntCube = addRenderable<GeometryBase, SimpleCube>(SimpleCube(1.0f), lShaderCube);
    auto &lRenderableCube = ECS.get<Renderable>(lEntCube);
    ECS.get<Transformation>(lEntCube).translate(0.0f, 0.0f, 5.0f);

    auto lEntCube1 = addRenderable<GeometryBase, SimpleCube>(SimpleCube(1.0f), lShaderCube);
    auto &lRenderableCube1 = ECS.get<Renderable>(lEntCube1);
    ECS.get<Transformation>(lEntCube1).translate(10.0f, 0.0f, -20.0f);

    //Würfel mit dynamischer Environment Map
    auto lShaderFBO = ShaderManager::getShader(QList<QString>(
        {lPath + QString("shader/dynenvmap.vert"), lPath + QString("shader/dynenvmap.frag")}));
    auto lEntFBO = addRenderable<GeometryBase, SimpleCube>(SimpleCube(10.0f), lShaderFBO);
    //Der letzte Parameter setzt einerseits die Texture Unit und gleichzeitig auch den Namen für die Textur
    auto lEntFBOTexture = std::make_shared<RenderToTexture>(lEntFBO, ColorCube);
    ECS.get<Renderable>(lEntFBO).addProperty(lEntFBOTexture);
    ECS.get<Transformation>(lEntFBO).translate(.0f, .0f, -20.0f);
    auto lKeyTrans = addController<KeyboardTransformationController>(lEntFBO);
    lKeyTrans->setRotKeysUpper('u', 'i', 'o');
    lKeyTrans->setTransKeysUpper('r', 't', 'z');

    //Plane, in die mit Defaultkamera gerendert wird
    auto lShaderFBOPlane = ShaderManager::getShader(QList<QString>(
        {lPath + QString("shader/texture.vert"), lPath + QString("shader/fbo_texture.frag")}));
    auto lEntFBOPlane = addRenderable<GeometryBase, SimplePlane>(SimplePlane(10.0f), lShaderFBOPlane);
    auto lEntFBOTexturePlane = std::make_shared<RenderToTexture>(lEntFBOPlane, Color2D);
    ECS.get<Renderable>(lEntFBOPlane).addProperty(lEntFBOTexturePlane);
    ECS.get<Transformation>(lEntFBOPlane).translate(0.0f, .0f, 30.0f);
    ECS.get<Transformation>(lEntFBOPlane).rotate(70.0f, QVector3D(1.0f, 0.0f, 0.0f));
    auto lKeyTransPlane = addController<KeyboardTransformationController>(lEntFBOPlane);
    lKeyTransPlane->setRotKeysUpper('j', 'k', 'l');
}
