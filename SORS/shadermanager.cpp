#include "manager.hpp"
#include "rendering/renderable.hpp"
#include "system.hpp"
#include "shadermanager.hpp"
#include "shader.hpp"

QHash<QString, std::shared_ptr<Shader>> ShaderManager::sShaders;
QDir ShaderManager::sDir;

void ShaderSystems::preRender() {
    auto lShader = *ShaderManager::getDefaultShader();
    for (auto &&[lEntity, lRenderable]: ECS.view<Renderable>(entt::exclude<Shader>).each()) {
        ECS.emplace<Shader>(lEntity, lShader);
    }
}

std::shared_ptr<Shader> ShaderManager::getDefaultShader()
{
    return ShaderManager::getShader(QList<QString>({QString(DEFAULTSHADERDIR)+ QString("/basic.vert"),
                                                    QString(DEFAULTSHADERDIR)+ QString("/basic.frag")}));
}
