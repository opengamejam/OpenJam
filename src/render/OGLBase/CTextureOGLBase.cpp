//
//  CTextureOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CTextureOGLBase.h"
#include "IImage.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTextureOGLBase::CTextureOGLBase()
    : m_Id(0)
    , m_Filter(ITexture::Linear)
    , m_IsDirty(true)
{
}

CTextureOGLBase::~CTextureOGLBase()
{
    glDeleteTextures(1, &m_Id);
}

void CTextureOGLBase::Bind()
{
    if (!IsValid()) {
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Id); // TODO: texture type
}

void CTextureOGLBase::Unbind()
{
    if (!IsValid()) {
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0); // TODO: texture type
}

void CTextureOGLBase::Filter(ITexture::TextureFilters filter)
{
    if (!IsValid()) {
        return;
    }

    m_Filter = filter;
    m_IsDirty = true;

    GLfloat parameter = TextureFilterToGlFilter(filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
    if (filter == ITexture::TextureFilters::Linear) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //assert(glGetError() == GL_NO_ERROR);
}

ITexture::TextureFilters CTextureOGLBase::Filter() const
{
    return m_Filter;
}

void CTextureOGLBase::Allocate(uint64_t width, uint64_t height)
{
    if (!IsValid()) {
        glGenTextures(1, &m_Id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    
    Bind();
    
    GLenum glInternalFormat = GL_RGBA;
    GLenum glFormat = GL_RGBA;
    GLenum glType = GL_UNSIGNED_BYTE;
    
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, nullptr);
    
    Filter(ITexture::TextureFilters::Linear);
    
    Unbind();
}

bool CTextureOGLBase::AssignImage(IImagePtr image)
{
    if (!IsValid()) {
        glGenTextures(1, &m_Id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    if (image) {
        Bind();

        assert(image && !image->RawData().empty());

        GLenum glInternalFormat = TexelFormatsToGlInternalFormat(image->TexelFormat());
        GLenum glFormat = TexelFormatsToGlFormat(image->TexelFormat());
        GLenum glType = TexelTypeToGlType(image->TexelType(), image->TexelFormat());

        uint32_t mipDataOffset = 0;
        for (uint32_t i = 0; i < image->MipsCount(); ++i) {
            uint32_t mipWidth = std::max<uint32_t>(image->Width() >> i, 1);
            uint32_t mipHeight = std::max<uint32_t>(image->Height() >> i, 1);
            uint32_t mipSize = std::max<uint32_t>(32, mipWidth * mipHeight * image->Bpp() / 8);

            if (image->IsCompressed()) {
                glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0,
                    (GLsizei)mipSize, &image->RawData()[mipDataOffset]);
            } else {
                glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType,
                    &image->RawData()[mipDataOffset]);
            }

            mipDataOffset += mipSize;
        }

        //assert(glGetError() == GL_NO_ERROR);

        if (image->MipsCount() > 1) {
            Filter(ITexture::TextureFilters::UseMipMaps);
        } else {
            Filter(ITexture::TextureFilters::Linear);
        }

        Unbind();

        return true;
    }

    return false;
}

const std::string& CTextureOGLBase::Hash()
{
    if (m_IsDirty) {
        HashMe();
        m_IsDirty = false;
    }

    return m_Hash;
}

GLuint CTextureOGLBase::TextureId() const
{
    return m_Id;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************
bool CTextureOGLBase::IsValid() const
{
    return (m_Id != 0);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CTextureOGLBase::HashMe()
{
    std::stringstream ss;
    ss << Filter();

    m_Hash = ss.str();
}

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
