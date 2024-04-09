#include "geometry/geometryindexedbase.hpp"
#include "manager.hpp"
#include "geometry/simpleplane.hpp"
#include "geometry/meshobj.hpp"
#include "rendering/rendercontext.hpp"
#include "rendering/rendertotexture.hpp"
#include <transformation/keyboardtransformationcontroller.hpp>
#include "shadermanager.hpp"
#include "rendering/texture.hpp"
#include "transformation/interpolatedrotation.hpp"
#include "transformation/relationship.hpp"
#include "vbuffer.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;

    auto lShaderGBufferVelo = ShaderManager::getShader(QList<QString>(
        {lPath + "shader/gbuffer.vert", lPath + "shader/gbuffer.frag"}));
    auto lShaderTexture =
        ShaderManager::getShader({lPath + "shader/texture.vert", lPath + "shader/texture.frag"});

    auto lMeshOBJ = MeshOBJ(lPath+QString("models/mario/mario-sculpture.obj"));
    auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShaderGBufferVelo);
    ECS.get<Transformation>(lEnt).scale(.1f,.1f,.1f);
    ECS.get<Transformation>(lEnt).translate(.0f, .0f,-10.f);
    auto lTexture = std::make_shared<Texture>(lPath + QString("models/mario/") +lMeshOBJ.getTextureDiffusePath(0), 4);
    auto &lRenderable = ECS.get<Renderable>(lEnt);
    lRenderable.addProperty(lTexture);

    //Property zum Setzen der für Velocity-Buffer notwendigen Uniforms (alte MVP) hinzufügen
    auto lVelocityProperty = std::make_shared<VBufferUniforms>(lEnt);
    lRenderable.addProperty(lVelocityProperty);

    auto &lEntOrbit = addTransformationEntity();
    lEntOrbit.translate(.0f, 5.f, .0f);

    auto &lEntTrafo = addTransformationEntity();
    auto lIPRotation = addController<InterpolatedRotation>(entt::to_entity(ECS, lEntTrafo));
    lIPRotation->rotateStart(90., QVector3D(.0, 0.0, 1.0));
    lIPRotation->rotateEnd(-90, QVector3D(.0, 0.0, 1.0));
    lIPRotation->setBackandForth(true);
    lIPRotation->setSpeed(0.01f);

    Relationship::addChild(entt::to_entity(ECS, lEntTrafo), entt::to_entity(ECS, lEntOrbit));
    Relationship::addChild(entt::to_entity(ECS, lEntOrbit), lEnt);

    QVector<TexRenderDestFormat> lTexImagesGBuffer{{GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0},
                                     {GL_RGB32F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1},
                                                   {GL_RGB, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT2}};

    auto lSceneBuffer = std::make_shared<RenderToTexture>(lEnt, Color2D, 0, FollowMainCam, QMatrix4x4(), lTexImagesGBuffer, PrePass, false);
    ECS.get<Renderable>(lEnt).setSingleRenderContext(lSceneBuffer->getContext());

    auto lShaderMotionBlur = ShaderManager::getShader(QList<QString>({lPath + QString("shader/motionblur.vert"), lPath + QString("shader/motionblur.frag")}));
    auto lEntMain = addRenderable<GeometryBase, SimplePlane>(SimplePlane(2.f), lShaderMotionBlur);
    ECS.get<Renderable>(lEntMain).setSingleRenderContext(GameSystem::getMainRenderContext());

    ECS.get<Renderable>(lEntMain).addProperty(lSceneBuffer);

    // auto lShaderSSAO = ShaderManager::getShader(QList<QString>(
    //     {lPath + QString("shader/ssao.vert"), lPath + QString("shader/ssao.frag")}));
    // auto lShaderFinal = ShaderManager::getShader(QList<QString>(
    //     {lPath + QString("shader/ssao.vert"), lPath + QString("shader/final.frag")}));

    // auto lSSAOPlane = addRenderable<GeometryBase, SimplePlane>(SimplePlane(2.0f), lShaderSSAO);
    // auto lFinalPlane = addRenderable<GeometryBase, SimplePlane>(SimplePlane(2.0f), lShaderFinal);

    // auto lKeyTrans = addController<KeyboardTransformationController>(lSSAOPlane);
    // lKeyTrans->setTransKeysUpper('j', 'k', 'l');
    // lKeyTrans->setRotKeysUpper('u', 'i', 'o');

    // auto lMeshOBJ = MeshOBJ(lPath+QString("models/scene.obj"));
    // auto lEnt = addRenderable<GeometryIndexedBase, MeshOBJ>(lMeshOBJ, lShaderSSAOGeo);

    // //ECS.get<Transformation>(lEnt).translate(0.0f, 0.0f, -10.0f);

    // //Offset Matrix für Kamera der Prerendered-Textur - wird hier nicht verwendet (=Id)
    // QMatrix4x4 lMat;

    // //Parameter für verschiedene Rendertargets des GBuffers festlegen
    // QVector<TexRenderDestFormat> lTexImagesGBuffer{{GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0},
    //                                  {GL_RGB16F, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT1},
    //                                  {GL_RGB, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT2}
    // };
    // auto lEntGBufferTex = std::make_shared<RenderToTexture>(lEnt, Color2D, 0, FollowMainCam, lMat, lTexImagesGBuffer, PrePass, false);
    // ECS.get<RenderContext>(lEntGBufferTex->getContext()).setPriority(1);
    // //Eigenen Shader für Prerenderingdurchlauf setzen
    // ECS.get<RenderContext>(lEntGBufferTex->getContext()).setOwnShader(lShaderSSAOGeo);
    // //Eigentliches Objekt (=Szene) ist nur für die Erstellung des GBuffer-Durchlaufs aktiv
    // ECS.get<Renderable>(lEnt).setSingleRenderContext(lEntGBufferTex->getContext());

    // QVector<TexRenderDestFormat> lTexImagesSSAO{{GL_RED, GL_RGB, GL_FLOAT, GL_COLOR_ATTACHMENT0}};
    // auto lSSAOBuffer = std::make_shared<RenderToTexture>(lSSAOPlane, Color2D, 0, FollowMainCam, lMat, lTexImagesSSAO, PrePass, false);
    // //Plane mit AO wird nur in einem Kontext und nach dem GBuffer gerendert
    // ECS.get<Renderable>(lSSAOPlane).setSingleRenderContext(lSSAOBuffer->getContext());
    // ECS.get<RenderContext>(lSSAOBuffer->getContext()).setPriority(2);

    // auto lNoiseAndKernel = std::make_shared<SSAONoiseAndKernel>(5);
    // ECS.get<Renderable>(lSSAOPlane).addProperty(lNoiseAndKernel);
    // ECS.get<Renderable>(lSSAOPlane).addProperty(lEntGBufferTex);
    // ECS.get<Renderable>(lFinalPlane).addProperty(lSSAOBuffer);
    // ECS.get<Renderable>(lFinalPlane).setIsEnabled(true);
    // ECS.get<Renderable>(lFinalPlane).setSingleRenderContext(GameSystem::getMainRenderContext());
}

