//
//  CRenderTargetTextureOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetTextureOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL2_0::CRenderTargetTextureOGL2_0()
: m_Id(0)
{

}

CRenderTargetTextureOGL2_0::~CRenderTargetTextureOGL2_0()
{

}

void CRenderTargetTextureOGL2_0::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetTextureOGL2_0::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetTextureOGL2_0::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetTextureOGL2_0::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetTextureOGL2_0::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

ITexturePtr CRenderTargetTextureOGL2_0::Texture() const
{
    return nullptr; // TODO
}

void CRenderTargetTextureOGL2_0::Texture(ITexturePtr texture)
{
    // TODO:
}

void CRenderTargetTextureOGL2_0::BindAsColorToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL2_0::BindAsDepthToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL2_0::BindAsStencilToFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0, // TODO: texture id
                           0);
}

void CRenderTargetTextureOGL2_0::UnbindAsColorFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL2_0::UnbindAsDepthFromFrameBuffer()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           0,
                           0);
}

void CRenderTargetTextureOGL2_0::UnbindAsStencilFromFrameBuffer()
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

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
