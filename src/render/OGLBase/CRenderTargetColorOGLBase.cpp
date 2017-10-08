//
//  CRenderTargetColorOGLBase.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetColorOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGLBase::CRenderTargetColorOGLBase()
    : m_Id(0)
    , m_InternalFormat(IRenderTarget::ColorRGBA8888)
{
}

CRenderTargetColorOGLBase::~CRenderTargetColorOGLBase()
{
}

void CRenderTargetColorOGLBase::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized()) {
        return;
    }
#if GL_ARB_framebuffer_object
    glGenRenderbuffers(1, &m_Id);
#else
    m_Id = 1;
#endif
    m_InternalFormat = internalFormat;
}

void CRenderTargetColorOGLBase::Shutdown()
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

bool CRenderTargetColorOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetColorOGLBase::Allocate(uint64_t width, uint64_t height)
{
#if GL_ARB_framebuffer_object
    glRenderbufferStorage(GL_RENDERBUFFER, ConvertInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#endif
}

void CRenderTargetColorOGLBase::Bind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
    JAM_LOG("CRenderTargetColorOGLBase::Bind() - id: %d\n", m_Id);
#endif
}

void CRenderTargetColorOGLBase::Unbind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif
    JAM_LOG("CRenderTargetColorOGLBase::Unbind() - id: %d\n", m_Id);
}

void CRenderTargetColorOGLBase::BindToFrameBuffer(uint64_t colorAttachementIdx)
{
#if GL_ARB_framebuffer_object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + (GLenum)colorAttachementIdx,
        GL_RENDERBUFFER,
        m_Id);
#endif
    JAM_LOG("CRenderTargetColorOGLBase::BindToFrameBuffer() - id: %d\n", m_Id);
}

void CRenderTargetColorOGLBase::UnbindFromFrameBuffer(uint64_t colorAttachementIdx)
{
#if GL_ARB_framebuffer_object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + (GLenum)colorAttachementIdx,
        GL_RENDERBUFFER,
        0);
#endif
    JAM_LOG("CRenderTargetColorOGLBase::UnbindFromFrameBuffer() - id: %d\n", m_Id);
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
