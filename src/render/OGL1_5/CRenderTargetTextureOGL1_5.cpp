//
//  CRenderTargetTextureOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CRenderTargetTextureOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL1_5::CRenderTargetTextureOGL1_5()
: m_Id(0)
{

}

CRenderTargetTextureOGL1_5::~CRenderTargetTextureOGL1_5()
{

}

void CRenderTargetTextureOGL1_5::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    glGenRenderbuffers(1, &m_Id);
}

void CRenderTargetTextureOGL1_5::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetTextureOGL1_5::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetTextureOGL1_5::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetTextureOGL1_5::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ITexturePtr CRenderTargetTextureOGL1_5::Texture() const
{
    return nullptr; // TODO
}

void CRenderTargetTextureOGL1_5::Texture(ITexturePtr texture)
{
    // TODO:
}

void CRenderTargetTextureOGL1_5::BindAsColorToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGL1_5::BindAsDepthToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGL1_5::BindAsStencilToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGL1_5::UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL1_5::UnbindAsDepthFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL1_5::UnbindAsStencilFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
