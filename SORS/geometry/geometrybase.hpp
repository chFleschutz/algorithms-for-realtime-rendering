// Geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "ecs.hpp"
#include "opengl.hpp"
#include "geometry/igeometryimplementation.hpp"

class Shader;

//! GeometryBase ist die Basisklasse für alle Objekte , die die geometrischen Daten eines zu zeichenenden
//! Objektes enthalten. Dazu gehören alle Vertex-Attribute, also auch Normalen,
//! Texturkoordinaten, etc. Eine Farbe pro Vertex kann ebenfalls festgelegt werden.
//! Jedes Renderable muss ein Geometry-Objekt haben, um gezeichnet werden zu können. Mehrere Renderables
//! können sich aber eine Geometry teilen!
//! Spezielle Geometrien wie SimpleCubes oder Meshes leiten nicht von Geometry ab, sondern implementieren das
//! Interface IGeometryImplementation
class GeometryBase
{
public:
    GeometryBase();
    GeometryBase(std::shared_ptr<IGeometryImplementation> pImplementation);
    virtual ~GeometryBase();

    //! Buffer für das Rendern binden
    void bindBuffer(Shader& shader);

    //! Buffer nach dem Rendern freigeben
    void unbindBuffer();

    //! Geometry-Basis Initialisieren. Wenn geometry noch nicht initialisiert wurde, werden die Geometriedaten befüllt sowie
    //! die Buffer erstellt und befüllt.
    void initializeGeometryBase();

    //! Buffer, Vertex Array Objects sowie alle Vertices, Colors, Normals und Textur Koordinaten löschen.
    void deInitGeometry();

    //! Getter für die Vertices der Geometry.
    const QVector<QVector4D>& getVertices() const {return mVertices;}

    //! Getter für die Normalen der Geometry.
    const QVector<QVector3D>& getNormals() const {return mVertexNormals;}

    //! Getter für die Textur-Koordinaten der Geometry.
    const QVector<QVector2D>& getTextureCoordinates() const {return mTextureCoordinates;}

    //! Getter für die Vertex-Farben der Geometry.
    const QVector<QVector4D>& getVertexColors() const {return mVertexColors;}

    //! Geometry ggf. initialisieren und Rendern an Implementation delegieren
    virtual void render();

    //! Gibt eine liste der Vertices der Geometry zurück bei der immer 3 Vertices ein Dreieck bilden. Bei normalen
    //! Geometrien, die als Dreiecksliste abgelegt sind, ist diese Methode equivalent zu getVertices(). Wenn die Daten
    //! jedoch indexed (IndexedGeometry) sind, gibt getTriangleList() die Vertices als Dreiecksliste
    //! zurück.
    virtual QVector<QVector4D> getTriangleList();

    //! Implementation setzen: Die eigentliche Geometrie liegt im ECS, die Implementierung mit Ihren Besonderheiten und ggf.
    //! Polymorphismus bleibt außerhalb
    virtual void setImplementation(std::shared_ptr<IGeometryImplementation> pImplementation);

    //! Implementation holen
    virtual std::shared_ptr<IGeometryImplementation> getImplementation() {return mImplementation;}

    QMatrix4x4 getmodelTransformation() const;
    void setModelTransformation(const QMatrix4x4 &pModelTransformation);

    //! Findet die passende Geometrie (Component von einer der Basisklassen in sRenderableGeometryClasses abgeleitet),
    //! die an pEnt hängt
    static GeometryBase *getGeometry(entt::entity pEnt);

    //! Legt für Entity pEnt die entsprechende Geometry an und verbindet diese mit der übergebenen Implementierung
    template <typename GeometryBaseClass = GeometryBase, typename IGeometryImplementationClass>
    static void emplaceToEntity(entt::entity pEnt, IGeometryImplementationClass pImplementation);

    //! Gibt die Klassen zurück, die als renderfähige Geometrie registriert wurden
    //! Meist GeometryBase und/oder GeometryIndexedBase
    static const QSet<entt::id_type> renderableGeometryClasses() {return GeometryBase::sRenderableGeometryClasses;}

protected:
    //! Wird beim Initialisieren der Geometrie und erstellt die Buffer. Kann überschriben werden um zusätzliche Buffer
    //! wie z.B. einen IndexBuffer zu erstellen.
    virtual void prepareVertexBuffers();

    //! Initialisieren ein Vertex Array Object für einen Shader.
    //! Wird pro Shader der diese Geometrie rendert einmal aufgerufen.
    //! Kann überschrieben werden um zusätzliche Buffer wie z.B. einen IndexBuffer an das Vertex Array Object
    //! anzuhängen.
    virtual void prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao);

    //! Zerstört alle Buffer der Geometrie. Kann überschrieben werden um zusätzliche Buffer wie z.B. einen IndexBuffer
    //! zu zerstören.
    virtual void destroyVertexBuffers();

    //! Hilfsmethode zur erstellung eines Buffers sowie senden der Daten an den Buffer.
    template <typename T>
    void initBuffer(QOpenGLBuffer& buffer, const QVector<T>& elements);

    //! Felder für Geometriedaten
    QVector<QVector4D> mVertices;
    QVector<QVector4D> mVertexColors;
    QVector<QVector3D> mVertexNormals;
    QVector<QVector2D> mTextureCoordinates;

    //! Matrix zur Speicherung der Modeltransformation (z.B. Skalierung)
    //! Diese Matrix wird nicht an etwaige angehängte Objekte o.ä. weitergegeben
    QMatrix4x4 mModelTransformation;

    //! Buffer für die Geometriedaten
    QOpenGLBuffer mVertexPositionBuffer;

    QOpenGLBuffer mVertexColorBuffer;
    QOpenGLBuffer mNormalBuffer;
    QOpenGLBuffer mTextureCoordinatesBuffer;

    //! Enthält die konkrete Implementierung der Geometry
    std::shared_ptr<IGeometryImplementation> mImplementation = nullptr;

    //! Speichert das in bindBuffer() gebundene QOpenGLVertexArrayObject zwischen, um es in unbindBuffer() wieder
    //! unbinden zu können.
    std::shared_ptr<QOpenGLVertexArrayObject> mCurrentVAO = nullptr;

    //!Muss für jede (von @see GeometryBase abgeleitete) Klasse aufgerufen werden, die gerendert werden soll
    template <typename RenderableClass>
    static void addRenderableClass();

    //! Enthält die entt Typen aller von GeometryBase abgeleitenen Klassen, also aller Klassen, die
    //! eine Geometrie für ein Renderable enthalten können
    static QSet<entt::id_type> sRenderableGeometryClasses;

private:
    //! Ein QOpenGLVertexArrayObject pro Shader da Attribute-Locations nicht konstant sein müssen und der Optimizer
    //! evtl. Vertex Attribute aus Shader, die diese nicht benötigen wegoptimiert.
    QHash<QString, std::shared_ptr<QOpenGLVertexArrayObject>> mVAOs;

};

template <typename GeometryBaseClass, typename IGeometryImplementationClass>
void GeometryBase::emplaceToEntity(entt::entity pEnt, IGeometryImplementationClass pImplementation)
{
    auto &lGeo1 = ECS.emplace_or_replace<GeometryBaseClass>(pEnt);
    lGeo1.setImplementation(std::make_shared<IGeometryImplementationClass>(pImplementation));
}

template <typename RenderableClass>
void GeometryBase::addRenderableClass()
{
    GeometryBase::sRenderableGeometryClasses.insert(entt::type_hash<RenderableClass>::value());
}

template <typename T>
void GeometryBase::initBuffer(QOpenGLBuffer& buffer, const QVector<T>& elements)
{
    if (!buffer.isCreated())
    {
        buffer.create();
    }

    buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    buffer.bind();
    buffer.allocate(elements.constData(), elements.size() * (int) sizeof(T));
}

#endif
