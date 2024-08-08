#pragma once

#include "rendering/irenderableproperty.hpp"
#include "input/inputregistry.hpp"

class TerrainColor : public IRenderableProperty
{
public:
	TerrainColor() = default;

	virtual void preRender(Shader& pShader)
	{
		keyboard();
		auto shaderProgram = pShader.shaderProgram();
		shaderProgram->setUniformValue("waterLevel", mWaterLevel);
		shaderProgram->setUniformValue("sandLevel", mSandLevel);
		shaderProgram->setUniformValue("grassLevel", mGrassLevel);
		shaderProgram->setUniformValue("snowLevel", mSnowLevel);
	}

private:
	void keyboard()
	{
		auto input = InputRegistry::getInstance().getKeyboardInput();
		// Check inputs
		if (input->isKeyPressed('1'))
			mWaterLevel += 0.001f;
		else if (input->isKeyPressed('2'))
			mWaterLevel -= 0.001f;
		else if (input->isKeyPressed('3'))
			mSandLevel += 0.001f;
		else if (input->isKeyPressed('4'))
			mSandLevel -= 0.001f;
		else if (input->isKeyPressed('5'))
			mGrassLevel += 0.001f;
		else if (input->isKeyPressed('6'))
			mGrassLevel -= 0.001f;
		else if (input->isKeyPressed('7'))
			mSnowLevel += 0.001f;
		else if (input->isKeyPressed('8'))
			mSnowLevel -= 0.001f;

		// Check boundaries of levels
		if (mWaterLevel < 0.0f)
			mWaterLevel = 0.0f;
		if (mWaterLevel > mSandLevel)
			mWaterLevel = mSandLevel - 0.01f;
		if (mSandLevel < mWaterLevel)
			mSandLevel = mWaterLevel + 0.01f;
		if (mSandLevel > mGrassLevel)
			mSandLevel = mGrassLevel - 0.01f;
		if (mGrassLevel < mSandLevel)
			mGrassLevel = mSandLevel + 0.01f;
		if (mGrassLevel > mSnowLevel)
			mGrassLevel = mSnowLevel - 0.01f;
		if (mSnowLevel < mGrassLevel)
			mSnowLevel = mGrassLevel + 0.01f;
		if (mSnowLevel > 1.0f)
			mSnowLevel = 1.0f;

		qDebug() << "Water: " << mWaterLevel << " Sand: " << mSandLevel << " Grass: " << mGrassLevel << " Snow: " << mSnowLevel;
	}

	float mWaterLevel = 0.4f;
	float mSandLevel = 0.45f;
	float mGrassLevel = 0.55f;
	float mSnowLevel = 0.65f;
};