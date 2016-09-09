//
//  CRenderTargetTextureOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#include "CRenderTargetTextureOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL1_3::CRenderTargetTextureOGL1_3()
: m_Id(0)
{

}

CRenderTargetTextureOGL1_3::~CRenderTargetTextureOGL1_3()
{

}

void CRenderTargetTextureOGL1_3::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetTextureOGL1_3::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetTextureOGL1_3::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetTextureOGL1_3::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetTextureOGL1_3::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ITexturePtr CRenderTargetTextureOGL1_3::Texture() const
{
    return nullptr; // TODO
}

void CRenderTargetTextureOGL1_3::Texture(ITexturePtr texture)
{
    // TODO:
}

void CRenderTargetTextureOGL1_3::BindAsColorToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL1_3::BindAsDepthToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL1_3::BindAsStencilToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL1_3::UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL1_3::UnbindAsDepthFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL1_3::UnbindAsStencilFromFrameBuffer()
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

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) */