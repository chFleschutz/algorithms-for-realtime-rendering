#include <map>
#include "geometrybase.hpp"
#include "shader.hpp"
#include "manager.hpp"
#include "win_oglbase/openglwidget.hpp"
#include "rendering/rendercontext.hpp"
#include "geometry/geometryshared.hpp"

QSet<entt::id_type> GeometryBase::sRenderableGeometryClasses;

GeometryBase::GeometryBase(): mModelTransformation(), mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
    mVertexColorBuffer(QOpenGLBuffer::VertexBuffer)
{
    GeometryBase::addRenderableClass<GeometryBase>();
}

GeometryBase::GeometryBase(std::shared_ptr<IGeometryImplementation> pImplementation):
    mModelTransformation(), mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
    mVertexColorBuffer(QOpenGLBuffer::VertexBuffer), mImplementation(pImplementation)
{
}

//! Findet im übergebenen Entity die (einzige!) Geometrie
//! Also die Component an der Entity, die eine von GeometryBase abgeleitete Klasse hat
//! Falls keine solche Component gefunden wird, wird nullptr zurückgegeben
GeometryBase* GeometryBase::getGeometry(entt::entity pEnt)
{
    if (ECS.all_of<SharedGeometry>(pEnt))
    {
        auto lSharedGeoEnt = ECS.storage<SharedGeometry>().get(pEnt);
        return GeometryBase::getGeometry(lSharedGeoEnt.getEntWithGeometryToShare());
    }
    else for (auto lCurrType: GeometryBase::renderableGeometryClasses())
        if ((ECS.storage(lCurrType) != nullptr) && ECS.storage(lCurrType)->contains(pEnt))
            return static_cast<GeometryBase*>(ECS.storage(lCurrType)->get(pEnt));
    return nullptr;
}

GeometryBase::~GeometryBase()
{
    deInitGeometry();
}

void GeometryBase::deInitGeometry()
{
    mVertices.clear();
    mVertexNormals.clear();
    mTextureCoordinates.clear();
    mVertexColors.clear();

    //VAOs selbst werden durch Destruktor (shared_ptr) freigegeben
    //Zum Freigeben der OpenGL-Ressourcen muss der OpenGL-Kontext aktiv sein
    //Durch die automatische Freigabe des Speichers ist die richtige Reihenfolge aber nicht
    //einzuhalten. Daher OpenGL-Ressourcen nur freigeben, wenn nicht der Kontext
    //freigegeben wird/wurde (=Programmende)
    if (!OpenGLWidget::getInstance()->getIsInCleanup()) {
        mVAOs.clear();
        destroyVertexBuffers();
    }
}

void GeometryBase::render()
{
    auto lShader = RenderContext::getActiveRenderContext().getActiveShader();
    bindBuffer(lShader);

    //Uniforms setzen
    if (!mVertexColors.empty())
        lShader.shaderProgram()->setUniformValue("hasColors", true);

    mImplementation->render();
}

void GeometryBase::setImplementation(std::shared_ptr<IGeometryImplementation> pImplementation)
{
    mImplementation = std::move(pImplementation);
}

QVector<QVector4D> GeometryBase::getTriangleList()
{
    return mVertices;
}

void GeometryBase::bindBuffer(Shader& shader)
{
    std::shared_ptr<QOpenGLVertexArrayObject> vao;
    if (mVAOs.contains(shader.getKey()))
        vao = mVAOs.value(shader.getKey());
    else
    {
        // Vertex array object für aktuellen shader existiert nicht => Erzeuge vao (Es muss 1 vao pro shader existieren)
        vao = std::make_shared<QOpenGLVertexArrayObject>();
        mVAOs.insert(shader.getKey(), vao);
        prepareVertexArrayObject(shader, *vao);
    }

    mCurrentVAO = vao;
    mCurrentVAO->bind();
}

void GeometryBase::unbindBuffer()
{
    Q_ASSERT_X(mCurrentVAO, "GeometryBase::unbindBuffer",
               "Unable to unbind buffer. Call GeometryBase::bindBuffer before you call this method");
    mCurrentVAO->release();
}

void GeometryBase::initializeGeometryBase()
{
    mImplementation->fillVertices(mVertices);
    mImplementation->fillColors(mVertexColors);
    mImplementation->fillNormals(mVertexNormals, mVertices);
    mImplementation->fillTexCoords(mTextureCoordinates);

    prepareVertexBuffers();
}

void GeometryBase::prepareVertexBuffers()
{
    if (!mVertices.isEmpty())
    {
        initBuffer(mVertexPositionBuffer, mVertices);
    }

    if (!mVertexColors.isEmpty())
    {
        initBuffer(mVertexColorBuffer, mVertexColors);
    }

    if (!mVertexNormals.isEmpty())
    {
        initBuffer(mNormalBuffer, mVertexNormals);
    }

    if (!mTextureCoordinates.isEmpty())
    {
        initBuffer(mTextureCoordinatesBuffer, mTextureCoordinates);
    }
}

void GeometryBase::prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao)
{
    // Create a VAO for this "object"
    vao.create();
    vao.bind();

    auto shaderProgram = shader.shaderProgram();
    if (!mVertices.isEmpty())
    {
        mVertexPositionBuffer.bind();
        shaderProgram->enableAttributeArray("vertexPosition");
        shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 4);
    }

    if (!mVertexColors.isEmpty())
    {
        mVertexColorBuffer.bind();
        shaderProgram->enableAttributeArray("vertexColor");
        shaderProgram->setAttributeBuffer("vertexColor", GL_FLOAT, 0, 4);
    }

    if (!mVertexNormals.isEmpty())
    {
        mNormalBuffer.bind();
        shaderProgram->enableAttributeArray("vertexNormal");
        shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);
    }

    if (!mTextureCoordinates.isEmpty())
    {
        mTextureCoordinatesBuffer.bind();
        shaderProgram->enableAttributeArray("textureCoords");
        shaderProgram->setAttributeBuffer("textureCoords", GL_FLOAT, 0, 2);
    }
}

void GeometryBase::destroyVertexBuffers()
{
    mVertexPositionBuffer.destroy();
    mVertexColorBuffer.destroy();
    mNormalBuffer.destroy();
    mTextureCoordinatesBuffer.destroy();
}

QMatrix4x4 GeometryBase::getmodelTransformation() const
{
    return mModelTransformation;
}

void GeometryBase::setModelTransformation(const QMatrix4x4 &pModelTransformation)
{
    mModelTransformation = pModelTransformation;
}
