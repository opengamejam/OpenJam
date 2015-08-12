//
//  CTextureOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CTextureOGL2_0.h"
#include "IImage.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

static const std::map<TexelFormats, int> s_GlInternalFormats = {
    {TF_A8,             GL_ALPHA},
    {TF_L8,             GL_LUMINANCE},
    {TF_LA88,           GL_LUMINANCE_ALPHA},
    {TF_RGB565,         GL_RGB},
    {TF_RGB888,         GL_RGB},
    {TF_RGBA5551,       GL_RGBA},
    {TF_RGBA4444,       GL_RGBA},
    {TF_RGBA8888,       GL_RGBA},
    {TF_RGBA32323232,   GL_RGBA},
    {TF_BGRA8888,       GL_RGBA},
#if GL_IMG_texture_compression_pvrtc
    {TF_PVRTC2,         GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG},
#endif // GL_IMG_texture_compression_pvrtc
#if GL_EXT_pvrtc_sRGB
    {TF_PVRTC2Alpha,    GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT},
    {TF_PVRTC4,         GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT},
    {TF_PVRTC4Alpha,    GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT}
#endif // GL_EXT_pvrtc_sRGB
};

static const std::map<TexelFormats, int> s_GlFormats = {
    {TF_A8,             GL_ALPHA},
    {TF_L8,             GL_LUMINANCE},
    {TF_LA88,           GL_LUMINANCE_ALPHA},
    {TF_RGB565,         GL_RGB},
    {TF_RGB888,         GL_RGB},
    {TF_RGBA5551,       GL_RGBA},
    {TF_RGBA4444,       GL_RGBA},
    {TF_RGBA8888,       GL_RGBA},
    {TF_RGBA32323232,   GL_RGBA},
    {TF_BGRA8888,       GL_BGRA},
#if GL_IMG_texture_compression_pvrtc
    {TF_PVRTC2,         GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG},
#endif // GL_IMG_texture_compression_pvrtc
#if GL_EXT_pvrtc_sRGB
    {TF_PVRTC2Alpha,    GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT},
    {TF_PVRTC4,         GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT},
    {TF_PVRTC4Alpha,    GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT}
#endif // GL_EXT_pvrtc_sRGB
};

static const std::map<TexelTypes , int> s_GlTypes = {
    {TT_UByte,          GL_UNSIGNED_BYTE},
    {TT_Byte,           GL_BYTE},
    {TT_UShort,         GL_UNSIGNED_SHORT},
    {TT_Short,          GL_SHORT},
#if RENDER_OGL2_0
    {TT_UInt,           GL_UNSIGNED_INT},
    {TT_Int,            GL_INT},
#endif
    {TT_Float,          GL_FLOAT}
};

static const std::map<ITexture::TextureFilters, float> s_GLFilters = {
    {ITexture::TextureFilters::Linear, GL_LINEAR},
    {ITexture::TextureFilters::Nearest, GL_NEAREST},
    {ITexture::TextureFilters::UseMipMaps, GL_LINEAR_MIPMAP_NEAREST}
};

INL float TextureFilterToGlFilter(ITexture::TextureFilters filter);
INL int TexelFormatsToGlInternalFormat(TexelFormats texelFormat);
INL int TexelFormatsToGlFormat(TexelFormats texelFormat);
INL int TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTextureOGL2_0::CTextureOGL2_0()
: m_Id(0)
, m_Filter(ITexture::Linear)
, m_IsDirty(true)
{
    
}

CTextureOGL2_0::~CTextureOGL2_0()
{
    glDeleteTextures(1, &m_Id);
}

void CTextureOGL2_0::Bind()
{
    if (!IsValid())
    {
        return;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Id);    // TODO: texture type
}

void CTextureOGL2_0::Unbind()
{
    if (!IsValid())
    {
        return;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);    // TODO: texture type
}

bool CTextureOGL2_0::IsValid() const
{
    return (m_Id != 0);
}

void CTextureOGL2_0::Filter(ITexture::TextureFilters filter)
{
    if (!IsValid())
    {
        return;
    }
    
    m_Filter = filter;
    m_IsDirty = true;
    
    GLfloat parameter = TextureFilterToGlFilter(filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
    if (filter == ITexture::TextureFilters::Linear)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

ITexture::TextureFilters CTextureOGL2_0::Filter() const
{
    return m_Filter;
}

bool CTextureOGL2_0::AssignImage(IImagePtr image)
{
    if (!IsValid())
    {
        glGenTextures(1, &m_Id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    
    if (image)
    {
        Bind();
        
        assert(image && !image->RawData().empty());
        
        int glInternalFormat = TexelFormatsToGlInternalFormat(image->TexelFormat());
        int glFormat = TexelFormatsToGlFormat(image->TexelFormat());
        int glType = TexelTypeToGlType(image->TexelType(), image->TexelFormat());
        
        size_t mipDataOffset = 0;
        for (unsigned int i = 0; i < image->MipsCount(); ++i)
        {
            unsigned int mipWidth = std::max<unsigned int>(image->Width() >> i, 1);
            unsigned int mipHeight = std::max<unsigned int>(image->Height() >> i, 1);
            size_t mipSize = std::max<size_t>(32, mipWidth * mipHeight * image->Bpp() / 8);
            
            if (image->IsCompressed())
            {
                glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0,
                                       (GLsizei)mipSize, &image->RawData()[mipDataOffset]);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType,
                             &image->RawData()[mipDataOffset]);
            }
            
            mipDataOffset += mipSize;
        }
        
        if (image->MipsCount() > 1)
        {
            Filter(ITexture::TextureFilters::UseMipMaps);
        }
        else
        {
            Filter(ITexture::TextureFilters::Linear);
        }
        
        Unbind();
        
        return true;
    }
    
    return false;
}

const std::string& CTextureOGL2_0::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CTextureOGL2_0::HashMe()
{
    std::stringstream ss;
    ss << Filter();
    
    m_Hash = ss.str();
}

INL float TextureFilterToGlFilter(ITexture::TextureFilters filter)
{
    GLfloat parameter = GL_LINEAR;
    
    std::map<ITexture::TextureFilters, float>::const_iterator iter = s_GLFilters.find(filter);
    assert(iter != s_GLFilters.end());
    if (iter != s_GLFilters.end())
    {
        parameter = iter->second;
    }
    
    return parameter;
}

INL int TexelFormatsToGlInternalFormat(TexelFormats texelFormat)
{
    std::map<TexelFormats, int>::const_iterator iter = s_GlInternalFormats.find(texelFormat);
    assert(iter != s_GlInternalFormats.end());
    
    return iter->second;
}

INL int TexelFormatsToGlFormat(TexelFormats texelFormat)
{
    std::map<TexelFormats, int>::const_iterator iter = s_GlFormats.find(texelFormat);
    assert(iter != s_GlFormats.end());
    
    return iter->second;
}

INL int TexelTypeToGlType(TexelTypes texelType, TexelFormats texelFormat)
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

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0
