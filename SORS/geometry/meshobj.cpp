#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include <QFileInfo>
#include <QDir>
#include "meshobj.hpp"
#include "manager.hpp"
#include "geometry/geometryindexedbase.hpp"

MeshOBJ::~MeshOBJ()
{

}

void MeshOBJ::fillVertices(QVector<QVector4D> &vertices)
{
    for (size_t i=0; i < mAttributesTiny.vertices.size()/3; i++)
        vertices.append(QVector4D(mAttributesTiny.vertices[i*3], mAttributesTiny.vertices[i*3+1], mAttributesTiny.vertices[i*3+2], 1.0f));
}

void MeshOBJ::fillNormals(QVector<QVector3D> &normals, QVector<QVector4D> &vertices)
{
    normals.resize(mAttributesTiny.vertices.size()/3);
    for (size_t s = 0; s < mShapesTiny.size(); s++) {

        size_t index_offset = 0;
        for (size_t f = 0; f < mShapesTiny[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(mShapesTiny[s].mesh.num_face_vertices[f]);
            Q_ASSERT(fv == 3);

            if (!mGenerateFaceNormals) {
                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = mShapesTiny[s].mesh.indices[index_offset + v];
                    if (idx.normal_index < 0) {
                        qDebug() << "Keine gültigen Daten für Normalen in .obj";
                        normals.clear();
                        return;
                    }

                    normals[idx.vertex_index]
                        = QVector3D(mAttributesTiny.normals[3 * size_t(idx.normal_index) + 0],
                                    mAttributesTiny.normals[3 * size_t(idx.normal_index) + 1],
                                    mAttributesTiny.normals[3 * size_t(idx.normal_index) + 2]);
                }
            } else {
                auto lNewell = [=]() -> QVector3D {
                    QVector3D out;
                    for (size_t i = 0; i < fv; i++) {
                        QVector3D lVertex_i = QVector3D(
                            mAttributesTiny
                                .vertices[mShapesTiny[s].mesh.indices[index_offset + i].vertex_index*3],
                            mAttributesTiny
                                .vertices[mShapesTiny[s].mesh.indices[index_offset + i].vertex_index*3
                                          + 1],
                            mAttributesTiny
                                .vertices[mShapesTiny[s].mesh.indices[index_offset + i].vertex_index*3
                                          + 2]);
                        QVector3D lVertex_iPlus1 = QVector3D(
                            mAttributesTiny
                                .vertices[mShapesTiny[s]
                                              .mesh.indices[index_offset + ((i + 1) % fv)]
                                              .vertex_index*3],
                            mAttributesTiny.vertices
                                [mShapesTiny[s].mesh.indices[index_offset + ((i + 1) % fv)].vertex_index*3
                                 + 1],
                            mAttributesTiny.vertices
                                [mShapesTiny[s].mesh.indices[index_offset + ((i + 1) % fv)].vertex_index*3
                                 + 2]);
                        out[0] += ((lVertex_i[1] - lVertex_iPlus1[1])
                                   * (lVertex_i[2] + lVertex_iPlus1[2]))
                                  / 2;
                        out[1] += ((lVertex_i[2] - lVertex_iPlus1[2])
                                   * (lVertex_i[0] + lVertex_iPlus1[0]))
                                  / 2;
                        out[2] += ((lVertex_i[0] - lVertex_iPlus1[0])
                                   * (lVertex_i[1] + lVertex_iPlus1[1]))
                                  / 2;
                    }
                    return (out.normalized());
                };
                auto lNormal = lNewell();
                for (size_t lFaceVertices = 0; lFaceVertices < fv; lFaceVertices++) {
                    normals[mShapesTiny[s].mesh.indices[index_offset + lFaceVertices].vertex_index]
                        = lNormal;
                }
            }
            index_offset += fv;
        }
    }
}

void MeshOBJ::fillTexCoords(QVector<QVector2D> &texCoords)
{
    texCoords.resize(mAttributesTiny.vertices.size()/3);
    for (size_t s = 0; s < mShapesTiny.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < mShapesTiny[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(mShapesTiny[s].mesh.num_face_vertices[f]);
            Q_ASSERT(fv == 3);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = mShapesTiny[s].mesh.indices[index_offset + v];
                if (idx.texcoord_index < 0) {
                    qDebug() << "Keine gültigen Daten für Texturkoordinaten in .obj";
                    texCoords.clear();
                    return;
                }
                texCoords[idx.vertex_index] = QVector2D(
                    mAttributesTiny.texcoords[2*size_t(idx.texcoord_index)+0],
                    mAttributesTiny.texcoords[2*size_t(idx.texcoord_index)+1]
                    );
            }
            index_offset += fv;
        }
    }
}

void MeshOBJ::fillIndices(QVector<GLuint> &indices)
{
    //Aus: https://github.com/tinyobjloader/tinyobjloader
    //! Hier wird bislang nur eine Shape geladen, auch nicht mit wechselndem Material
    //! Ein komplettes Laden von .obj-Files würde mehrere Renderables erzeugen, jedes mit seinem eigenen Material
    for (size_t s = 0; s < mShapesTiny.size(); s++) {
        // Loop over faces(polygon)
//        Q_ASSERT(mShapesTiny.size() == 1);

        size_t index_offset = 0;
        for (size_t f = 0; f < mShapesTiny[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(mShapesTiny[s].mesh.num_face_vertices[f]);
            Q_ASSERT(fv == 3);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = mShapesTiny[s].mesh.indices[index_offset + v];
                indices.append(idx.vertex_index);
            }
            index_offset += fv;

            // TODO Materialien auslesen
            mMaterialID = mShapesTiny[s].mesh.material_ids[f];
        }
    }
    mNrOfIndices = indices.size();
}

bool MeshOBJ::getGenerateFaceNormals() const
{
    return mGenerateFaceNormals;
}

void MeshOBJ::setGenerateFaceNormals(bool pGenerateFaceNormals)
{
    mGenerateFaceNormals = pGenerateFaceNormals;
}

void MeshOBJ::init()
{
    tinyobj::ObjReaderConfig llReaderCFG;
    auto lDir = QFileInfo(mFilePath);
    llReaderCFG.mtl_search_path = lDir.path().toStdString();
    tinyobj::ObjReader lReader;

    if (!lReader.ParseFromFile(mFilePath.toStdString(), llReaderCFG)) {
        if (!lReader.Error().empty()) {
            qDebug() << "TinyObjlReader: " << lReader.Error().c_str();
        }
        exit(1);
    }

    if (!lReader.Warning().empty()) {
        qDebug() << "TinyObjlReader: " << lReader.Warning().c_str();
    }

    mAttributesTiny = lReader.GetAttrib();
    mShapesTiny = lReader.GetShapes();
    mMaterialsTiny = lReader.GetMaterials();
    mNeedsInit = false;
}

QString MeshOBJ::getTextureDiffusePath(unsigned pMaterialID)
{
    init();
    if (mMaterialsTiny.size() <= pMaterialID)
        return QString();
    else
        return (QString(mMaterialsTiny[pMaterialID].diffuse_texname.c_str())); //mMaterialID].diffuse_texname.c_str()));
}
