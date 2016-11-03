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
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetStencilOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }

    glDeleteRenderbuffers(1, &m_Id);
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

    glRenderbufferStorage(GL_RENDERBUFFER, ConvertToInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetStencilOGLBase::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetStencilOGLBase::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetStencilOGLBase::BindToFrameBuffer()
{
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == Depth24_Stencil8) {
        attachement = GL_DEPTH_ATTACHMENT;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        attachement,
        GL_RENDERBUFFER,
        m_Id);
}

void CRenderTargetStencilOGLBase::UnbindFromFrameBuffer()
{
    GLenum attachement = GL_STENCIL_ATTACHMENT;
    if (m_InternalFormat == Depth24_Stencil8) {
        attachement = GL_DEPTH_ATTACHMENT;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        attachement,
        GL_RENDERBUFFER,
        0);
}

void CRenderTargetStencilOGLBase::InitializeWithDepthId(uint32_t depthId)
{
    m_InternalFormat = Depth24_Stencil8;
    m_Id = depthId;
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
