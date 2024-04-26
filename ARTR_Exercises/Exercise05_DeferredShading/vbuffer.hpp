#ifndef VBUFFER_HPP
#define VBUFFER_HPP
#include "rendering/irenderableproperty.hpp"
#include "rendering/rendercontext.hpp"
#include "geometry/geometrybase.hpp"
#include "transformation/transformation.hpp"
#include <rendering/pointlight.hpp>

class VBufferUniforms: public IRenderableProperty {
public:
    VBufferUniforms(entt::entity pOwnerEntity): mOwnerEntity(pOwnerEntity) {}

    void init(Shader& pShader) override {
        auto view = ECS.view<PointLight>();
        int i = 1;
        for (auto entity : view) {
            auto& light = view.get<PointLight>(entity);
            light.setDiffuse(QVector3D(0.01 * i, 0.01 * i, 0.01 * i));
            i++;
            // Konkateniere die Zeichenfolgen und die Zahl
        }
    }
    
    void preRender(Shader &pShader) override {
        auto view = ECS.view<PointLight>();
        int i = 0;
        for (auto entity : view) {
            auto& light = view.get<PointLight>(entity);
            const char* prefix = "lights[";
            const char* suffix = "].";
            std::string result = std::string(prefix) + std::to_string(i++) + suffix;
            std::string ambient = "ambient";
            std::string diffuse = "diffuse";
            std::string specular = "specular";
            std::string viewPosition = "viewPosition";
            std::string attenuations = "attenuations";
            // Konkateniere die Zeichenfolgen und die Zahl

            pShader.shaderProgram()->setUniformValue((result + ambient).c_str(), light.getAmbient());
            pShader.shaderProgram()->setUniformValue((result + diffuse).c_str(), light.getDiffuse());
            pShader.shaderProgram()->setUniformValue((result + specular).c_str(), light.getSpecular());
            pShader.shaderProgram()->setUniformValue((result + viewPosition).c_str(), light.getViewPosition());
            pShader.shaderProgram()->setUniformValue((result + attenuations).c_str(), light.getAttenuations());
        }
            
        
    }
protected:
    entt::entity mOwnerEntity;
    
};

#endif // VBUFFER_HPP
