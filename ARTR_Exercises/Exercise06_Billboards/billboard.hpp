#ifndef BILLBOARD_HPP
#define BILLBOARD_HPP

#include "geometry/igeometryimplementation.hpp"

class Billboard : public IGeometryImplementation
{
public:
	explicit Billboard(QList<QVector4D> pPoints)
	{
		mPoints = std::move(pPoints);
	}

	virtual void render() override
	{
		glFunctions->glDrawArrays(GL_POINTS, 0, mPoints.length());
	}

	virtual void fillVertices(QVector<QVector4D>& vertices) override
	{
		vertices.clear();
		vertices.reserve(mPoints.size());
		for (const auto& point : mPoints)
		{
			vertices.append(point);
		}
	}

private:
	QList<QVector4D> mPoints;
};

#endif // BILLBOARD_HPP
