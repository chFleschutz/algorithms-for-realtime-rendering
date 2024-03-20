#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>
#include "opengl.hpp"

//! Hier werden die Shaderprogramme erzeugt. Die Klasse besteht einerseits aus
//! "Convenience"-Methoden zum Laden und Übersetzen der Shader-Quelldateien. Außerdem wird für das Binden der Programme
//! zum richtigen Zeitpunkt sowie das Setzen der Uniform-Variablen gesorgt.
class Shader
{
public:
    Shader();
    virtual ~Shader();
    //! Aktivieren dieses Shaderprogramms
    virtual void enableShader();
    //! Setzen der Uniforms des Shaders, muss gerufen werden, wenn dieses Shaderprogramm aktiv ist
    virtual void setShaderUniforms();
    //! Deaktivieren dieses Shaderprogramms
    void disableShader();

    auto shaderProgram() {return mShaderProgram;}

    void listActiveUniforms();
    bool isValid() {return mCompLinkOK;}

    QString getKey() const;
    void setKey(const QString &pKey);

protected:
    //! Wird an verschiedenen Stellen benutzt, um einen Shader zu identifizieren
    //! Wird vom ShaderManager vergeben
    QString mKey;
    std::shared_ptr<QOpenGLShaderProgram> mShaderProgram;
    void compileLinkShader(const QList<QString> pShaderFilePaths);

    bool mCompLinkOK;
    auto loadShaderFile(QString pPath);

private:
    void initialize();
    void initialize(const QVector<QString> pShaderFilePaths);
    static QString sStringToPrepend;

    friend class ShaderManager;
};

#endif // SHADER_H
