#ifndef DEFERREDLIGHTS_HPP
#define DEFERREDLIGHTS_HPP
#include "rendering/irenderableproperty.hpp"
#include "rendering/rendercontext.hpp"
#include "geometry/geometrybase.hpp"
#include "transformation/transformation.hpp"
#include <rendering/pointlight.hpp>

class DeferredLights : public IRenderableProperty {
public:
	DeferredLights(entt::entity pOwnerEntity) : mOwnerEntity(pOwnerEntity) {
	}



	void init(Shader& pShader) override {

	}

	void preRender(Shader& pShader) override {
		for (int i = 0; i < mLightsNumber; ++i) {
			const char* prefix = "lights[";
			const char* suffix = "].";
			std::string result = std::string(prefix) + std::to_string(i) + suffix;
			std::string ambient = "ambient";
			std::string diffuse = "diffuse";
			std::string specular = "specular";
			std::string viewPosition = "viewPosition";
			std::string attenuations = "attenuations";

			// Konkateniere die Zeichenfolgen und die Zahl
			pShader.shaderProgram()->setUniformValue((result + ambient).c_str(), mLights[i].getAmbient());
			pShader.shaderProgram()->setUniformValue((result + diffuse).c_str(), mLights[i].getDiffuse());
			pShader.shaderProgram()->setUniformValue((result + specular).c_str(), mLights[i].getSpecular());
			pShader.shaderProgram()->setUniformValue((result + viewPosition).c_str(), mLights[i].getViewPosition());
			pShader.shaderProgram()->setUniformValue((result + attenuations).c_str(), mLights[i].getAttenuations());
		}
		auto lCameraPos = RenderContext::getActiveRenderContext().getCamera().getPosition();
		pShader.shaderProgram()->setUniformValue("actualLightsNumber", mLightsNumber);
		pShader.shaderProgram()->setUniformValue("viewPos", lCameraPos);
	}

	void SetLights(std::vector<PointLight> lights)
	{

		for (size_t i = 0; i < lights.size(); i++)
		{
			mLights[i] = lights[i];
		}
		mLightsNumber = lights.size();
	}
protected:
	entt::entity mOwnerEntity;
	int mLightsNumber = 0;
	PointLight mLights[1000];

};

#endif // VBUFFER_HPP
