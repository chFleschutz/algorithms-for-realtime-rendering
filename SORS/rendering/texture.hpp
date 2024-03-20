#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <QImage>
#include <QMap>
#include "irenderableproperty.hpp"
#include "textureimageformat.hpp"

//! Klasse zum Rendering von Texturen
class Texture: public IRenderableProperty
{
public:
    Texture(int unit = 0) {initMembers();mTextureUnit = unit;}
    Texture(const QString& pfad, int unit = 0);

    //! Bild aus Datei laden
    void loadPicture(QString pfad);

    //! Von IRenderableProperty abgeleitete Methoden implementieren
    virtual void init(Shader &shader) override;
    //! Rendering der Textur aktivieren
    virtual void preRender(Shader &shader) override;
    //! Rendering der Textur ausschalten
    virtual void postRender(Shader &shader) override;

    void setUnit(int unit) {mTextureUnit = unit;}
    void setDimMode(int mode);
    void setMaxFilter(int mode) {mMaxFilter = mode;}
    void setMinFilter(int mode) {mMinFilter = mode;}
    void setWrapMode(int mode) {mWrapMode = mode;}
    void setAnisotropyLevel(unsigned int level) {mAnisotropyLevel = std::max((int)std::min((unsigned int)mMaximumAnisotropy, level), 1);}
    void setNameString(QString pName) {mTexNameString=pName;}
    virtual void makeStripes(int size, int thickness, bool buildmipmaps = true);
    void makeDummy();
    virtual ~Texture();
    void setForceInitialize(bool pForceInitialize) {mForceInitialize = pForceInitialize;}

    //! Gibt true zurück wenn das geladene Bild einen Alpha-Kanal besitzt
    bool hasAlphaChannel() const;

    QImage::Format getqImageConversionFormat() const;
    void setQImageConversionFormat(const QImage::Format &pQImageConversionFormat);

    TexImageFormat gettexImageFormat() const;
    void setTexImageFormat(const TexImageFormat &pTexImageFormat);

protected:
    //! Textur initialisieren und laden - im Fehlerfall Dummy erzeugen
    virtual void textureInit(bool buildmipmaps = true);

    //! Maps bauen und in OpenGL laden
    virtual void loadImageToGPU(bool buildmipmaps = true);

    //! Dummy Textur erzeugen
    void initMembers();

    void convertImageToOpenGL(std::shared_ptr<QImage> pImage);

    GLuint mTexName;
    QString mTexNameString;
    int mTexDim;
    int mTextureUnit;
    TexImageFormat mTexImageFormat{GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE};
    QImage::Format mQImageConversionFormat{QImage::Format_RGBA8888};
    int mMinFilter;
    int mMaxFilter;
    int mWrapMode;
    float mMaximumAnisotropy;
    unsigned int mAnisotropyLevel;
    bool mInitialized;
    bool mForceInitialize;
    std::shared_ptr<QImage> mImage;     // Benutzt bei TEXTURE_2D
    std::shared_ptr<QImage> mImages[6]; // Benutzt fuer CubeTexture
    bool mTexImageDefined{false};
};

#endif // TEXTURE_HPP
