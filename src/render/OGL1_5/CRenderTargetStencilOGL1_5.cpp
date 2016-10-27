//
//  CRenderTargetStencilOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CRenderTargetStencilOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGL1_5::CRenderTargetStencilOGL1_5()
: m_Id(0)
, m_InternalFormat(0)
{

}

CRenderTargetStencilOGL1_5::~CRenderTargetStencilOGL1_5()
{

}

void CRenderTargetStencilOGL1_5::Initialize(InternalFormats internalFormat)
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

void CRenderTargetStencilOGL1_5::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetStencilOGL1_5::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetStencilOGL1_5::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetStencilOGL1_5::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetStencilOGL1_5::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetStencilOGL1_5::BindToFrameBuffer()
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

void CRenderTargetStencilOGL1_5::UnbindFromFrameBuffer()
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

void CRenderTargetStencilOGL1_5::InitializeWithDepthId(uint32_t depthId)
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

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
