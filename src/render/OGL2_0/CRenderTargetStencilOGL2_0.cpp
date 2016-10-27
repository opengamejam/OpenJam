//
//  CRenderTargetStencilOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetStencilOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGL2_0::CRenderTargetStencilOGL2_0()
: m_Id(0)
, m_InternalFormat(0)
{

}

CRenderTargetStencilOGL2_0::~CRenderTargetStencilOGL2_0()
{

}

void CRenderTargetStencilOGL2_0::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    
    if ((internalFormat & Stencil8) == Stencil8)
    {
        glGenRenderbuffers(1, &m_Id);
        m_InternalFormat = GL_STENCIL_INDEX8;
    }
}

void CRenderTargetStencilOGL2_0::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetStencilOGL2_0::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetStencilOGL2_0::Allocate(uint64_t width, uint64_t height)
{
    if (m_InternalFormat != GL_STENCIL_INDEX8)
    {
        return;
    }
    
    glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetStencilOGL2_0::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetStencilOGL2_0::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetStencilOGL2_0::BindToFrameBuffer()
{
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == GL_DEPTH24_STENCIL8_OES)
    {
        attachement = GL_DEPTH_ATTACHMENT;
    }
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              attachement,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetStencilOGL2_0::UnbindFromFrameBuffer()
{
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == GL_DEPTH24_STENCIL8_OES)
    {
        attachement = GL_DEPTH_ATTACHMENT;
    }
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              attachement,
                              GL_RENDERBUFFER,
                              0);
}

void CRenderTargetStencilOGL2_0::InitializeWithDepthId(uint32_t depthId)
{
    m_InternalFormat =  GL_DEPTH24_STENCIL8_OES;
    m_Id = depthId;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
