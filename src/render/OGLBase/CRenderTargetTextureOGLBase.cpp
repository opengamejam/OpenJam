//
//  CRenderTargetTextureOGLBase.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetTextureOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGLBase::CRenderTargetTextureOGLBase()
: m_Id(0)
{

}

CRenderTargetTextureOGLBase::~CRenderTargetTextureOGLBase()
{

}

void CRenderTargetTextureOGLBase::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    glGenRenderbuffers(1, &m_Id);
}

void CRenderTargetTextureOGLBase::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetTextureOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetTextureOGLBase::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetTextureOGLBase::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ITexturePtr CRenderTargetTextureOGLBase::Texture() const
{
    return nullptr; // TODO
}

void CRenderTargetTextureOGLBase::Texture(ITexturePtr texture)
{
    // TODO:
}

void CRenderTargetTextureOGLBase::BindAsColorToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGLBase::BindAsDepthToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGLBase::BindAsStencilToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           m_Id);
}

void CRenderTargetTextureOGLBase::UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGLBase::UnbindAsDepthFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGLBase::UnbindAsStencilFromFrameBuffer()
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

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
