//
//  CRenderTargetStencilOGLBase.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetStencilOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGLBase::CRenderTargetStencilOGLBase()
    : m_Id(0)
    , m_InternalFormat(IRenderTarget::Stencil8)
{
}

CRenderTargetStencilOGLBase::~CRenderTargetStencilOGLBase()
{
}

void CRenderTargetStencilOGLBase::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized()) {
        return;
    }

    m_InternalFormat = internalFormat;
    if ((internalFormat & Stencil8) == Stencil8) {
#if GL_ARB_framebuffer_object
        glGenRenderbuffers(1, &m_Id);
#else
        m_Id = 1;
#endif
    }
}

void CRenderTargetStencilOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }

#if GL_ARB_framebuffer_object
    glDeleteRenderbuffers(1, &m_Id);
#else
    m_Id = 0;
#endif
}

bool CRenderTargetStencilOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetStencilOGLBase::Allocate(uint64_t width, uint64_t height)
{
    if (m_InternalFormat != Stencil8) {
        return;
    }

#if GL_ARB_framebuffer_object
    glRenderbufferStorage(GL_RENDERBUFFER, ConvertToInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#endif
}

void CRenderTargetStencilOGLBase::Bind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
#endif
    JAM_LOG("CRenderTargetStencilOGLBase::Bind() - id: %d\n", m_Id);
}

void CRenderTargetStencilOGLBase::Unbind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif
    JAM_LOG("CRenderTargetStencilOGLBase::Unbind() - id: %d\n", m_Id);
}

IRenderTarget::InternalFormats CRenderTargetStencilOGLBase::InternalFormat() const
{
    return m_InternalFormat;
}

void CRenderTargetStencilOGLBase::BindToFrameBuffer()
{
#if GL_ARB_framebuffer_object
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == Depth24_Stencil8) {
        attachement = GL_DEPTH_ATTACHMENT;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        attachement,
        GL_RENDERBUFFER,
        m_Id);
#endif
    JAM_LOG("CRenderTargetStencilOGLBase::BindToFrameBuffer() - id: %d\n", m_Id);
}

void CRenderTargetStencilOGLBase::UnbindFromFrameBuffer()
{
#if GL_ARB_framebuffer_object
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == Depth24_Stencil8) {
        attachement = GL_DEPTH_ATTACHMENT;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        attachement,
        GL_RENDERBUFFER,
        0);
#endif
    JAM_LOG("CRenderTargetStencilOGLBase::UnbindFromFrameBuffer() - id: %d\n", m_Id);
}

void CRenderTargetStencilOGLBase::InitializeWithDepthId(uint32_t depthId)
{
#if GL_ARB_framebuffer_object
    m_InternalFormat = Depth24_Stencil8;
    m_Id = depthId;
#endif
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
