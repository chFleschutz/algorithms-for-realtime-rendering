#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP


#include <QRgb>
#include <QImage>
#include "geometry/geometrybase.hpp"
#include "input/inputregistry.hpp"
#include "ecs.hpp"

#include "geometry/igeometryimplementation.hpp"

class Heightmap : public IGeometryImplementation
{
public:
	Heightmap() : myScale(5.0), mPoints(false)
	{
		image_pfade[0] = "sam.bmp";
		image_pfade[1] = "logo.bmp";
		image_pfade[2] = "sw.bmp";
		image_pfade[3] = "terrain.bmp";
		image_pfade[4] = "swklein.bmp";
		loadImage(image_pfade[0]);
	}


	void loadImage(QString pPath)
	{
		auto lPath = QString(DEFAULT_IMAGES_DIR) + pPath;
		Q_ASSERT(image.load(lPath));
	}

	void setPoints(bool p) { mPoints = p; }

	//!Methoden aus @see IGeometryImplementation überschreiben
	virtual void render() override
	{
		keyboard();
		if (mPoints)
		{
			glFunctions->glPointSize(5.0);
			glFunctions->glDrawArrays(GL_POINTS, 0, 4 * (image.height() - 1) * (image.width() - 1));
		}
		else
			glFunctions->glDrawArrays(GL_TRIANGLES, 0, 6 * (image.height() - 1) * (image.width() - 1));
	}


protected:
	virtual void fillVertices(QVector<QVector4D>& vertices) override
	{
		int i, j;

		vertices.clear();
		for (i = 0; i < image.height() - 1; i++)
			for (j = 0; j < image.width() - 1; j++)
			{
				//Dreieck links oben
				QRgb farbe_ji = image.pixel(j, i);
				QRgb farbe_jiplus1 = image.pixel(j, i + 1);
				QRgb farbe_jplus1i = image.pixel(j + 1, i);
				QRgb farbe_jplus1iplus1 = image.pixel(j + 1, i + 1);
				vertices.append(QVector4D(scaleTo(i) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_ji)) / 10.0, scaleTo(j), 1.0));
				if (!mPoints)
					vertices.append(QVector4D(scaleTo(i) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_jplus1i)) / 10.0, scaleTo(j + 1), 1.0));
				vertices.append(QVector4D(scaleTo(i + 1) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_jiplus1)) / 10.0, scaleTo(j), 1.0));
				//Dreieck rechts unten
				if (!mPoints)
					vertices.append(QVector4D(scaleTo(i) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_jplus1i)) / 10.0, scaleTo(j + 1), 1.0));
				vertices.append(QVector4D(scaleTo(i + 1) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_jplus1iplus1)) / 10.0, scaleTo(j + 1), 1.0));
				vertices.append(QVector4D(scaleTo(i + 1) - scaleTo(image.height()) / 2, scaleTo(qGray(farbe_jiplus1)) / 10.0, scaleTo(j), 1.0));
			}
	}


	void fillColors(QVector<QVector4D>& colors) override
	{
		int i, j;

		colors.clear();
		for (i = 0; i < image.height() - 1; i++)
			for (j = 0; j < image.width() - 1; j++)
			{
				//Dreieck links oben
				QRgb farbe_ji = image.pixel(j, i);
				QRgb farbe_jiplus1 = image.pixel(j, i + 1);
				QRgb farbe_jplus1i = image.pixel(j + 1, i);
				QRgb farbe_jplus1iplus1 = image.pixel(j + 1, i + 1);
				colors.append(QVector4D(clampColor(qRed(farbe_ji)), clampColor(qGreen(farbe_ji)), clampColor(qBlue(farbe_ji)), 1.0));
				if (!mPoints)
					colors.append(QVector4D(clampColor(qRed(farbe_jplus1i)), clampColor(qGreen(farbe_jplus1i)), clampColor(qBlue(farbe_jplus1i)), 1.0));
				colors.append(QVector4D(clampColor(qRed(farbe_jiplus1)), clampColor(qGreen(farbe_jiplus1)), clampColor(qBlue(farbe_jiplus1)), 1.0));
				//Dreieck rechts unten
				if (!mPoints)
					colors.append(QVector4D(clampColor(qRed(farbe_jplus1i)), clampColor(qGreen(farbe_jplus1i)), clampColor(qBlue(farbe_jplus1i)), 1.0));
				colors.append(QVector4D(clampColor(qRed(farbe_jplus1iplus1)), clampColor(qGreen(farbe_jplus1iplus1)), clampColor(qBlue(farbe_jplus1iplus1)), 1.0));
				colors.append(QVector4D(clampColor(qRed(farbe_jiplus1)), clampColor(qGreen(farbe_jiplus1)), clampColor(qBlue(farbe_jiplus1)), 1.0));
			}
	}

	virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices)  override
	{
		normals.clear();

		if (mPoints) //Falls Punkte gezeichnet werden -> keine Normalen berechnen
			return;

		auto count = 3;
		Q_ASSERT(vertices.length() % count == 0);

		QVector<QVector3D> newellVertices;
		for (auto& vertex : vertices)
		{
			newellVertices.append(QVector3D(vertex));

			if (newellVertices.length() < count)
				continue;

			auto normal = newell(newellVertices);
			for (int i = 0; i < count; i++)
			{
				normals.append(normal);
			}
			newellVertices.clear();
		}

		//Für "weicheres" Aussehen Mittelwerte berechnen aus mehr Vertices
		if (mSmoothEnabled)
		{
			for (int i = 0; i < image.height() - 1; i++)
			{
				for (int j = 0; j < image.width() - 1; j++)
				{
					QList<int> normalIndices;
					int currentIndex = i * (image.width() - 1) * 6 + j * 6;

					bool isFirstRow = currentIndex < ((image.width() - 1) * 6);
					bool isFirstColumn = currentIndex % ((image.width() - 1) * 6) == 0;
					if (isFirstRow || isFirstColumn)
						continue;

					normalIndices.append(currentIndex);
					normalIndices.append(currentIndex - 3);
					normalIndices.append(currentIndex - 5);
					normalIndices.append(currentIndex - (image.width() - 1) * 6 + 2);
					normalIndices.append(currentIndex - (image.width() - 1) * 6 + 5);
					normalIndices.append(currentIndex - (image.width()) * 6 + 4);

					QVector3D smoothedNormal;
					for (int normalIndex : normalIndices)
						smoothedNormal += normals[normalIndex];
					smoothedNormal.normalize();

					for (int normalIndex : normalIndices)
						normals[normalIndex] = smoothedNormal;
				}
			}
		}
	}

	void keyboard()
	{
		auto keyIn = InputRegistry::getInstance().getKeyboardInput();
		mNeedsInit = true;

		if (keyIn->isKeyPressed('g'))
			myScale /= 1.1F;
		else if (keyIn->isKeyPressed('G'))
			myScale *= 1.1F;
		else if (keyIn->isKeyPressed('p'))
			mPoints = true;
		else if (keyIn->isKeyPressed('P'))
			mPoints = false;
		else if (keyIn->isKeyPressed('1'))
			loadImage(image_pfade[0]);
		else if (keyIn->isKeyPressed('2'))
			loadImage(image_pfade[1]);
		else if (keyIn->isKeyPressed('3'))
			loadImage(image_pfade[2]);
		else if (keyIn->isKeyPressed('4'))
			loadImage(image_pfade[3]);
		else if (keyIn->isKeyPressed('h'))
			mSmoothEnabled = false;
		else if (keyIn->isKeyPressed('H'))
			mSmoothEnabled = true;
		else
			mNeedsInit = false;
	}


private:
	QImage image;
	QString image_pfade[5];
	float myScale;
	bool mPoints;
	bool mSmoothEnabled{ false };

	float clampColor(uchar d) { return (d / 255.0); }

	double scaleTo(int toscale)
	{
		static int imageausdehnung = std::max(image.height(), image.width());

		return double(toscale) / imageausdehnung * myScale;
	}

	QVector3D newell(QList<QVector3D> pInputVertices)
	{
		auto vertexCount = pInputVertices.length();

		QVector3D result;
		for (int i = 0; i < vertexCount; i++)
		{
			auto& first = pInputVertices[i];
			auto& second = pInputVertices[(i + 1) % vertexCount];

			auto x = (first.y() - second.y()) * (first.z() + second.z());
			auto y = (first.z() - second.z()) * (first.x() + second.x());
			auto z = (first.x() - second.x()) * (first.y() + second.y());

			result += QVector3D(x, y, z);
		}

		return result.normalized();
	}

};

#endif // HEIGHTMAP_HPP
