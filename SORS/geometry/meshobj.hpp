#ifndef MESHOBJ_H
#define MESHOBJ_H

#include "geometry/igeometryindexedimplementation.hpp"
#include "thirdparty/tinyobjloader/tiny_obj_loader.h"

//! Sehr einfache Implementierung einer Mesh-Klasse, die aus .obj-Dateien (z.B. aus Blender) geladen wird.
//! Zum Laden wird tinyobjloader (siehe thirdparty) verwendet.
//! Vorsicht: Es werden derzeit nur Meshes geladen, die aus einem Teil bestehen. Weitere Einschränkunen sind,
//! dass nur die diffuse Textur und keine Materialien geladen werden.

class MeshOBJ: public IGeometryIndexedImplementation
{
public:
    MeshOBJ(): mFilePath() {}
    MeshOBJ(QString pFilePath): mFilePath(pFilePath) {}
    virtual ~MeshOBJ();
    void init() override;
    QString getTextureDiffusePath(unsigned int pMaterialID);

    bool getGenerateFaceNormals() const;
    void setGenerateFaceNormals(bool pGenerateFaceNormals);

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;
    virtual void fillIndices(QVector<GLuint>& indices) override;

private:
    tinyobj::attrib_t mAttributesTiny;
    std::vector<tinyobj::shape_t> mShapesTiny;
    std::vector<tinyobj::material_t> mMaterialsTiny;
    QString mFilePath;
    int mMaterialID;
    //! Erlaubt das Generierung von Flächennormalen, z.B. wenn das Modell keine Normalen mitliefert
    bool mGenerateFaceNormals{false};
};

#endif // MESHOBJ_H
