#include <QImage>
#include <QDir>
#include <QColor>

#include "texture.hpp"

void Texture::initMembers()
{
    mImage = NULL;
    mTexDim = GL_TEXTURE_2D;
    mMinFilter = GL_LINEAR_MIPMAP_LINEAR;
    mMaxFilter = GL_LINEAR;
    mWrapMode = GL_REPEAT;
    mInitialized = false;
    mForceInitialize = true;
    mTextureUnit = 0;
    mTexNameString = QString("textureMap");
}

void Texture::convertImageToOpenGL(std::shared_ptr<QImage> pImage)
{
    pImage->convertTo(mQImageConversionFormat);
    pImage->mirror();
}

TexImageFormat Texture::gettexImageFormat() const
{
    return mTexImageFormat;
}

void Texture::setTexImageFormat(const TexImageFormat &pTexImageFormat)
{
    mTexImageFormat = pTexImageFormat;
}

QImage::Format Texture::getqImageConversionFormat() const
{
    return mQImageConversionFormat;
}

void Texture::setQImageConversionFormat(const QImage::Format &pQImageConversionFormat)
{
    mQImageConversionFormat = pQImageConversionFormat;
}

void Texture::setDimMode(int mode)
{
    mTexDim = mode;
}

void Texture::loadImageToGPU(bool buildmipmaps)
{
    GLenum faces[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

    if (mTexDim != GL_TEXTURE_CUBE_MAP)
    {
        convertImageToOpenGL(mImage);
        if (!mTexImageDefined)
            glFunctions->glTexImage2D(mTexDim, 0, mTexImageFormat.mInternalFormat, mImage->width(), mImage->height(), 0, mTexImageFormat.mFormat, mTexImageFormat.mDataType, mImage->bits());
        else
            glFunctions->glTexSubImage2D(mTexDim, 0, 0, 0, mImage->width(), mImage->height(), mTexImageFormat.mFormat, mTexImageFormat.mDataType, mImage->bits());
    }
    else // GL_TEXTURE_CUBE_MAP
    {
        for (int i = 0; i < 6; i++)
        {
            convertImageToOpenGL(mImages[i]);
            if (!mTexImageDefined)
                glFunctions->glTexImage2D(faces[i], 0, mTexImageFormat.mInternalFormat, mImages[i]->width(), mImages[i]->height(), 0, mTexImageFormat.mFormat, mTexImageFormat.mDataType, mImages[i]->bits());
            else
                glFunctions->glTexSubImage2D(faces[i], 0, 0, 0, mImage->width(), mImage->height(), mTexImageFormat.mFormat, mTexImageFormat.mDataType, mImage->bits());
        }
    }

    if (buildmipmaps)
    {
        glFunctions->glGenerateMipmap(mTexDim);
    }
    mTexImageDefined = true;
}

void Texture::textureInit(bool buildmipmaps)
{
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);

    if (!mInitialized)
    {
        glFunctions->glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaximumAnisotropy);
        mAnisotropyLevel = mMaximumAnisotropy;

        glFunctions->glGenTextures(1, &mTexName);
        glFunctions->glBindTexture(mTexDim, mTexName);

        // optain the maximum texture size
        GLint texSize;
        glFunctions->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

        std::shared_ptr<QImage> test = (mImage == NULL) ? mImages[0] : mImage;
        if (test == NULL)
        {
            qFatal("No valid texture image loaded.");
        }
        if (test->width() > texSize || test->height() > texSize)
        {
            qDebug() << "Texture is too large!";
            qDebug() << "Maximum dimension size is: " << texSize;
        }
        qDebug() << "Texture " << test->width() << " x " << test->height() << " loaded.";

        glFunctions->glGetError(); // clear the error flag

        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MAG_FILTER, mMaxFilter);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_MIN_FILTER, mMinFilter);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_S, mWrapMode);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_T, mWrapMode);
        glFunctions->glTexParameteri(mTexDim, GL_TEXTURE_WRAP_R, mWrapMode);
        this->loadImageToGPU(buildmipmaps);

        glFunctions->glBindTexture(mTexDim, 0);
        mInitialized = true;
    }
}

Texture::Texture(const QString &pfad, int unit): mTextureUnit(unit)
{
    initMembers();
    mTextureUnit = unit;
    this->loadPicture(pfad);
}

void Texture::loadPicture(QString pfad)
{
    // Im Folgenden sind posy und negy ABSICHTLICH vertauscht:
    // Die Koordinatenachsen bei OpenGL Cubemaps stimmen nicht mit den typischen Cubemaps (Renderman, DirectX, ...)
    // überein
    // siehe hierzu GLSpec 4.5 Core, Seite 240

    QString fileNames[] = {"posx.png", "negx.png", "negy.png", "posy.png", "posz.png", "negz.png"};

    QDir directory;

    if (QDir::isRelativePath(pfad))
    {
        // try directories above... (macht nur Sinn bei relativen pfaden)
        for (int i = 0; i < 10; i++)
        {
            if (!directory.exists(pfad))
            {
                pfad.prepend("../");
            }
            else
            {
                break;
            }
        }
    }

    QDir folderOrFile(pfad);
    if (folderOrFile.count() > 0) // Versuche Cubemap zu laden
    {
        if (pfad[pfad.length()-1] != '/')
        {
            pfad.append("/");
        }
        for (int i = 0; i < 6; i++)
        {
            mImages[i] = std::make_shared<QImage>(pfad + fileNames[i]);
            if (mImages[i]->isNull())
            {
                qDebug() << "Failure loading CubeMap " << fileNames[i] << " in " << pfad;
            }
        }
        mTexDim = GL_TEXTURE_CUBE_MAP;
#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
        glFunctions->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif
    }
    else // TEXTURE_2D
    {
        mImage = std::make_shared<QImage>(pfad);
        if (mImage->isNull())
        {
            qDebug() << "Failed to load " << pfad;
            makeDummy();
        }
    }
}

void Texture::init(Shader &shader)
{
    textureInit();
}

void Texture::makeStripes(int size, int thickness, bool buildmipmaps)
{
    int i, j;
    bool isWhite = true;

    mImage = std::make_shared<QImage>(size, size, QImage::Format_RGBA8888);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            QRgb color;
            if (j % thickness == 0)
            {
                isWhite = !isWhite;
            }
            if (isWhite)
            {
                color = qRgb(255, 255, 255);
            }
            else
            {
                color = qRgb(0, 0, 0);
            }
            mImage->setPixel(j, i, color);
        }
        isWhite = true;
    }

    this->textureInit(buildmipmaps);
}

void Texture::makeDummy()
{
    int i, j;
    float ti, tj;
    int height = 64, width = height;
    QColor color;

    mImage = std::make_shared<QImage>(width, height, QImage::Format_RGBA8888);

    for (i = 0; i < width; i++)
    {
        ti = 2.0 * 3.14159265 * i / width;
        for (j = 0; j < height; j++)
        {
            tj = 2.0 * 3.14159265 * j / height;

            color.setRed((uchar)127 * (1.0 + sin(ti)));
            color.setGreen((uchar)127 * (1.0 + cos(2 * tj)));
            color.setBlue((uchar)127 * (1.0 + cos(ti + tj)));
            mImage->setPixel(i, j, color.rgb());
        }
    }
}

void Texture::preRender(Shader &shader)
{
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
    if (mForceInitialize && !mInitialized)
    {
        textureInit();
    }

    glFunctions->glBindTexture(mTexDim, mTexName);
    shader.shaderProgram()->setUniformValue((mTexNameString.toStdString()).c_str(), (GLuint)mTextureUnit);
    //Flag setzen, an dem das Shaderprogramm erkennt, dass eine Textur vorhanden ist
    shader.shaderProgram()->setUniformValue((QString("has").append(mTexNameString)).toStdString().c_str(), true);
}

void Texture::postRender(Shader &shader)
{
    shader.shaderProgram()->setUniformValue((QString("has").append(mTexNameString)).toStdString().c_str(), false);
    glFunctions->glActiveTexture(GL_TEXTURE0 + mTextureUnit);
    glFunctions->glBindTexture(mTexDim, 0);
}

Texture::~Texture()
{
    if (mTexName)
    {
        glFunctions->glDeleteTextures(1, &mTexName);
    }
}

bool Texture::hasAlphaChannel() const
{
    if (mImage)
    {
        return mImage->hasAlphaChannel();
    }

    if (mImages[0])
    {
        for (int i = 0; i < 6; ++i)
        {
            if (mImages[i]->hasAlphaChannel())
            {
                return true;
            }
        }

        return false;
    }

    return false;
}
