#include "win_oglbase/openglwidget.hpp"
#include "manager.hpp"
#include "geometry/simplecube.hpp"
#include "geometry/geometryindexedbase.hpp"
#include "geometry/simplesphere.hpp"
#include "transformation/transformation.hpp"
#include "transformation/keyboardtransformationcontroller.hpp"
#include "transformation/relationship.hpp"
#include "rendering/rendercontext.hpp"

void Manager::initialize()
{
    QString lPath = SRCDIR;
    QString lSORSPATH(SORSDIR);

    auto lShader =
        ShaderManager::getShader(QList<QString>(
            {lSORSPATH + QString("shader/basic.vert"),
             lSORSPATH + QString("shader/basicred.frag")}));

    entt::entity lEnt, lFirstEnt, lLastEnt;

    for (auto i=0; i<10; i++) {
        if (i%2)
            lEnt = addRenderable<GeometryBase, SimpleCube>(SimpleCube());
        else
            lEnt = addRenderable<GeometryBase, SimpleCube>(SimpleCube(), lShader);
        auto &lTrans = ECS.get<Transformation>(lEnt);
        lTrans.translate(i/5.5f,0.,0.);
        if (i==0) {
            lFirstEnt = lLastEnt =  lEnt;
            auto lKeyTrans = addController<KeyboardTransformationController>(lEnt);
            lKeyTrans->setTransKeysUpper('j', 'k', 'l');
            lKeyTrans->setMoveSpeed(1.f/10.f);
            lKeyTrans->setRotSpeed(1.f/3.f);
        }
        else
        {
            if (i==5) {
                auto lKeyTrans = addController<KeyboardTransformationController>(lEnt);
                lKeyTrans->setTransKeysUpper('f', 'g', 'h');
                lKeyTrans->setMoveSpeed(1.f/10.f);
                lKeyTrans->setRotSpeed(1.f/3.f);
            }
            Relationship::addChild(lLastEnt, lEnt);
            lLastEnt = lEnt;
        }
    }
    addRenderable<GeometryIndexedBase, SimpleSphere>(SimpleSphere(), lShader);

    addRenderableSharedGeometry(lLastEnt, lShader);

//    for (auto &&curr: sRegistry.storage()) //entt::type_id<Shader>().hash()))
//        qDebug() << "Test." << curr.first << " " << curr.second.type().name().;
}
