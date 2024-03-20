#include <QFile>
#include "opengl.hpp"
#include "shader.hpp"
#include "rendering/rendercontext.hpp"

//! String, der am Anfang aller Shader eingefügt wird, z.B., um die Version einheitlich einzufügen
QString Shader::sStringToPrepend("#version 400 \n");

Shader::Shader():mShaderProgram(new QOpenGLShaderProgram()){}

Shader::~Shader(){}

auto Shader::loadShaderFile(const QString pPath)
{
    QFile lShaderFile(pPath);
    // If path is not in the execution folder move up and try again for max 10 times
    for (int i = 0; i < 10; i++)
    {
        if (!lShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString lPath = pPath;
            lPath.insert(0, "../");
            lShaderFile.setFileName(lPath);
        }
        else
            break;
    }

    if (!lShaderFile.isOpen())
    {
        qDebug() << "could not open" << pPath << "shader source file. returned empty string.";
        return QString();
    }
    else
    {
        QString lSourceCode;
        lSourceCode = lShaderFile.readAll();
        lShaderFile.close();
        return lSourceCode.prepend(sStringToPrepend);
    }
}

void Shader::compileLinkShader(const QList<QString> pShaderFilePaths)
{
    const QOpenGLShader::ShaderType lShaderTypes[] = {
        QOpenGLShader::Vertex, QOpenGLShader::Fragment, QOpenGLShader::Geometry,
        QOpenGLShader::TessellationControl, QOpenGLShader::TessellationEvaluation
    };

    unsigned i = 0;
    mCompLinkOK = true;

    for (auto lShaderPath: pShaderFilePaths)
    {
        QString lSourceCode;

        lSourceCode = loadShaderFile(lShaderPath);

        if (!lSourceCode.isEmpty())
        {
            if(!mShaderProgram->addShaderFromSourceCode(lShaderTypes[i], lSourceCode))
            {
                mCompLinkOK = false;
                qDebug() << "Failed to compile " << lShaderTypes[i] << " shader";
                qDebug() << "File: " << pShaderFilePaths[i];
                qDebug() << "Log: " << shaderProgram()->log();
                return;
            }
        }
        i++;
    }

    if (!mShaderProgram->link())
    {
        mCompLinkOK = false;
        qDebug() << "Failed to link shader! Log: " << mShaderProgram->log();
        return;
    }
}

void Shader::initialize(const QVector<QString> pShaderFilePaths)
{
    compileLinkShader(pShaderFilePaths);
}

void Shader::enableShader()
{
    if (mCompLinkOK)
    {
        if (!shaderProgram()->bind())
        {
            qDebug() << "Shader program konnte nicht gebunden werden.";
        }
    }
}

//! Setzen aller (!) Uniforms des Shaders. Abgeleitete Klassen sollten diese Methode überschreiben,
//! um ihre eigenen Uniforms zu setzen. Gegebenenfalls dann die Methode dieser Basisklasse rufen, um
//! die "Standard"-Uniforms setzen zu lassen (Lichtquellen, etc.)
void Shader::setShaderUniforms()
{
// TODO    SceneManager::instance()->getActiveContext()->setUniforms(shaderProgram());
}

void Shader::disableShader()
{
    shaderProgram()->release();
}

void Shader::listActiveUniforms()
{
    int total = -1;
    glFunctions->glGetProgramiv(this->shaderProgram()->programId(), GL_ACTIVE_UNIFORMS, &total);
    qDebug() << "Listing " << total << " active uniforms";
    for (int i = 0; i < total; ++i)
    {
        int name_len = -1, num = -1;
        GLenum type = GL_ZERO;
        char name[100];
        glFunctions->glGetActiveUniform(this->shaderProgram()->programId(), GLuint(i), sizeof(name) - 1, &name_len,
                                        &num, &type, name);
        name[name_len] = 0;
        qDebug() << name;
    }
}

QString Shader::getKey() const
{
    return mKey;
}

void Shader::setKey(const QString &pKey)
{
    mKey = pKey;
}
