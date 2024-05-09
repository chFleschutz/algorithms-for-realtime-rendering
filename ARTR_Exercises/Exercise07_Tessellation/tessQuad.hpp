#pragma once

#include "geometry/igeometryimplementation.hpp"

class TessQuad : public IGeometryImplementation
{
public:
	TessQuad() = default;
	explicit TessQuad(float size) : mSize(size) {}

	virtual void fillVertices(QVector<QVector4D>& vertices) override
	{
		vertices.clear();

		auto halfSize = mSize / 2.0f;
		vertices.append({ -halfSize, -halfSize, 0.0f, 1.0f });
		vertices.append({ halfSize, -halfSize, 0.0f, 1.0f });
		vertices.append({ -halfSize, halfSize, 0.0f, 1.0f });
		vertices.append({ halfSize, halfSize, 0.0f, 1.0f });
	}

	virtual void fillTexCoords(QVector<QVector2D>& texCoords) override
	{
		texCoords.clear();

		texCoords.append({ 0.0f, 0.0f });
		texCoords.append({ 1.0f, 0.0f });
		texCoords.append({ 0.0f, 1.0f });
		texCoords.append({ 1.0f, 1.0f });
	}

	virtual void render() override
	{
		glFunctions->glPatchParameteri(GL_PATCH_VERTICES, 4);
		glFunctions->glDrawArrays(GL_PATCHES, 0, 4);
	}

private:
	float mSize = 1.0f;
};