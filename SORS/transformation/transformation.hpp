#ifndef TRANSFORMATIONBASE_H
#define TRANSFORMATIONBASE_H

#include <QMatrix4x4>
#include <QQuaternion>

//! Basisklasse für Transformations-Klassen implementiert die eigentlichen Transformationen.
class Transformation
{
public:
    Transformation(): mMatrix(), mWorldMatrix() {}
    Transformation(QMatrix4x4 pMat);

    //! Verschieben um Vektor, der durch die 3 Parameter gebildet wird
    void translate(float tx, float ty, float tz);

    //! Rotieren um die Achse \param axis um den Winkel \param angle
    void rotate(float angle, const QVector3D& axis);

    //! Rotieren um ein Quaternion
    void rotateQuat(QQuaternion pQuat);

    //! Rotieren
    void rotate(float angle, float x, float y, float z);

    //! Skalieren
    void scale(const QVector3D& scale);

    //! Skalieren
    void scale(float sx, float sy, float sz);

    //! Transformationen zurücksetzen (LoadIdentity)
    void resetTrafo();

    //! Setter für die ModelMatrix.
    void setMatrix(const QMatrix4x4& modelMatrix);

    //! Mutable getter für die ModelMatrix.
    inline QMatrix4x4& getMatrix() {return mMatrix;}

    //! Const getter für die ModelMatrix.
    inline const QMatrix4x4& getMatrix() const {return mMatrix;}

    QMatrix4x4 getWorldMatrix() const;
    void setWorldMatrix(const QMatrix4x4 &pWorldMatrix);

protected:
    //! Die Transformationen werden in dieser ModelMatrix gespeichert.
    QMatrix4x4 mMatrix;
    //! Enthält die ggf. in einer Hierarchie angesammelte Transformationsmatrix, die dann auf
    //! das zugehörige Entity und/oder Kindknoten angewandt wird
    //! Entspricht der WorldMatrix _ohne_ die Viewmatrix, die von der Kamera kommt
    //! Wird auch beim Durchlaufen von Relationships benutzt,
    //! um den akkumulierten Zwischenstand an Kindknoten weiterzugeben
    //! @see TransformationHierarchyVisitor
    QMatrix4x4 mWorldMatrix;
};

#endif // TRANSFORMATIONBASE_H
