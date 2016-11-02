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

    glGenRenderbuffers(1, &m_Id);
    m_InternalFormat = internalFormat;
}

void CRenderTargetColorOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }

    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetColorOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetColorOGLBase::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, ConvertInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetColorOGLBase::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetColorOGLBase::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetColorOGLBase::BindToFrameBuffer(uint64_t colorAttachementIdx)
{
    Bind();
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
        GL_RENDERBUFFER,
        m_Id);
    
    Unbind();
}

void CRenderTargetColorOGLBase::UnbindFromFrameBuffer(uint64_t colorAttachementIdx)
{
    Bind();
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
        GL_RENDERBUFFER,
        0);
    
    Unbind();
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
