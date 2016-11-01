//
//  CTextureOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CTextureOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

static const std::map<TexelFormats, int> s_GlInternalFormats = {
    { TF_A8, GL_ALPHA },
    { TF_L8, GL_LUMINANCE },
    { TF_LA88, GL_LUMINANCE_ALPHA },
    { TF_RGB565, GL_RGB },
    { TF_RGB888, GL_RGB },
    { TF_RGBA5551, GL_RGBA },
    { TF_RGBA4444, GL_RGBA },
    { TF_RGBA8888, GL_RGBA },
    { TF_RGBA32323232, GL_RGBA },
#ifdef GL_BGRA
    { TF_BGRA8888, GL_RGBA },
#endif
#ifdef GL_RGB565_TWID
    { TF_RGB565_TWID, GL_RGB565_TWID },
#endif // GL_RGB565_TWID
#ifdef GL_ARGB1555_TWID
    { TF_ARGB1555_TWID, GL_ARGB1555_TWID },
#endif // GL_ARGB1555_TWID
#ifdef GL_ARGB4444_TWID
    { TF_ARGB4444_TWID, GL_ARGB4444_TWID },
#endif // GL_ARGB4444_TWID
#if GL_IMG_texture_compression_pvrtc
    { TF_PVRTC2, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG },
#endif // GL_IMG_texture_compression_pvrtc
#if GL_EXT_pvrtc_sRGB
    { TF_PVRTC2Alpha, GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT },
    { TF_PVRTC4, GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT },
    { TF_PVRTC4Alpha, GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT }
#endif // GL_EXT_pvrtc_sRGB
};

static const std::map<TexelFormats, int> s_GlFormats = {
    { TF_A8, GL_ALPHA },
    { TF_L8, GL_LUMINANCE },
    { TF_LA88, GL_LUMINANCE_ALPHA },
    { TF_RGB565, GL_RGB },
    { TF_RGB888, GL_RGB },
    { TF_RGBA5551, GL_RGBA },
    { TF_RGBA4444, GL_RGBA },
    { TF_RGBA8888, GL_RGBA },
    { TF_RGBA32323232, GL_RGBA },
#ifdef GL_BGRA
    { TF_BGRA8888, GL_BGRA },
#endif // GL_RGBA
#ifdef GL_RGB565_TWID
    { TF_RGB565_TWID, GL_RGB565_TWID },
#endif // GL_RGB565_TWID
#ifdef GL_ARGB1555_TWID
    { TF_ARGB1555_TWID, GL_ARGB1555_TWID },
#endif // GL_ARGB1555_TWID
#ifdef GL_ARGB4444_TWID
    { TF_ARGB4444_TWID, GL_ARGB4444_TWID },
#endif // GL_ARGB4444_TWID
#if GL_IMG_texture_compression_pvrtc
    { TF_PVRTC2, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG },
#endif // GL_IMG_texture_compression_pvrtc
#if GL_IMG_texture_compression_pvrtc
    { TF_PVRTC2, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG },
#endif // GL_IMG_texture_compression_pvrtc
#if GL_EXT_pvrtc_sRGB
    { TF_PVRTC2Alpha, GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT },
    { TF_PVRTC4, GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT },
    { TF_PVRTC4Alpha, GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT }
#endif // GL_EXT_pvrtc_sRGB
};

static const std::map<TexelTypes, int> s_GlTypes = {
    { TT_UByte, GL_UNSIGNED_BYTE },
    { TT_Byte, GL_BYTE },
    { TT_UShort, GL_UNSIGNED_SHORT },
    { TT_Short, GL_SHORT },
#ifdef GL_UNSIGNED_INT
    { TT_UInt, GL_UNSIGNED_INT },
#endif // GL_UNSIGNED_INT
#ifdef GL_INT
    { TT_Int, GL_INT },
#endif // GL_INT
    { TT_Float, GL_FLOAT }
};

static const std::map<ITexture::TextureFilters, float> s_GLFilters = {
    { ITexture::TextureFilters::Linear, GL_LINEAR },
    { ITexture::TextureFilters::Nearest, GL_NEAREST },
    { ITexture::TextureFilters::UseMipMaps, GL_LINEAR_MIPMAP_NEAREST }
};

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTextureOGLES1_0::CTextureOGLES1_0()
{
    glEnable(GL_TEXTURE_2D);
}

CTextureOGLES1_0::~CTextureOGLES1_0()
{
}

GLfloat CTextureOGLES1_0::TextureFilterToGlFilter(ITexture::TextureFilters filter)
{
    GLfloat parameter = GL_LINEAR;

    std::map<ITexture::TextureFilters, float>::const_iterator iter = s_GLFilters.find(filter);
    assert(iter != s_GLFilters.end());
    if (iter != s_GLFilters.end()) {
        parameter = iter->second;
    }

    return parameter;
}

GLenum CTextureOGLES1_0::TexelFormatsToGlInternalFormat(TexelFormats texelFormat)
{
    std::map<TexelFormats, int>::const_iterator iter = s_GlInternalFormats.find(texelFormat);
    assert(iter != s_GlInternalFormats.end());

    return iter->second;
}

GLenum CTextureOGLES1_0::TexelFormatsToGlFormat(TexelFormats texelFormat)
{
    std::map<TexelFormats, int>::const_iterator iter = s_GlFormats.find(texelFormat);
    assert(iter != s_GlFormats.end());

    return iter->second;
}

GLenum CTextureOGLES1_0::TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat)
{
    int glType = GL_UNSIGNED_BYTE;
    switch (texelFormat) {
    case TF_RGB565:
        glType = GL_UNSIGNED_SHORT_5_6_5;
        break;

    case TF_RGBA4444:
        glType = GL_UNSIGNED_SHORT_4_4_4_4;
        break;

    case TF_RGBA5551:
        glType = GL_UNSIGNED_SHORT_5_5_5_1;
        break;

    default:
        std::map<TexelTypes, int>::const_iterator iter = s_GlTypes.find(texelType);
        assert(iter != s_GlTypes.end());

        glType = iter->second;
        break;
    }

    return glType;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
