#ifndef TEXTUREIMAGEFORMAT_HPP
#define TEXTUREIMAGEFORMAT_HPP

#include "opengl.hpp"

struct TexImageFormat
{
    GLint mInternalFormat;
    GLenum mFormat;
    GLenum mDataType;
};

#endif // TEXTUREIMAGEFORMAT_HPP
