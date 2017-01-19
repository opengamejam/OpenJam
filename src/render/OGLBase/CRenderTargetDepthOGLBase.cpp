//
//  CRenderTargetDepthOGLBase.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetDepthOGLBase.h"
#include "CRenderTargetStencilOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGLBase::CRenderTargetDepthOGLBase()
    : m_Id(0)
    , m_InternalFormat(IRenderTarget::Depth24)
    , m_Stencil(nullptr)
{
}

CRenderTargetDepthOGLBase::~CRenderTargetDepthOGLBase()
{
    if (m_Stencil) {
        DeleteStencil();
    }
}

void CRenderTargetDepthOGLBase::Initialize(InternalFormats internalFormat)
{
    assert(!IsInitialized());

    switch (internalFormat) {
    case Depth24_Stencil8:
        if (!m_Stencil) {
            CreateStencil();
        }
        break;

    default:
        if (m_Stencil) {
            DeleteStencil();
        }
        break;
    };
#if GL_ARB_framebuffer_object
    glGenRenderbuffers(1, &m_Id);
#else
    m_Id = 1;
#endif
}

void CRenderTargetDepthOGLBase::Shutdown()
{
    assert(IsInitialized());
#if GL_ARB_framebuffer_object
    glDeleteRenderbuffers(1, &m_Id);
#else
    m_Id = 0;
#endif
}

bool CRenderTargetDepthOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGLBase::Allocate(uint64_t width, uint64_t height)
{
#if GL_ARB_framebuffer_object
    glRenderbufferStorage(GL_RENDERBUFFER, ConvertToInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#endif
}

void CRenderTargetDepthOGLBase::Bind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
#endif
    JAM_LOG("CRenderTargetDepthOGLBase::Bind() - id: %d\n", m_Id);
}

void CRenderTargetDepthOGLBase::Unbind() const
{
#if GL_ARB_framebuffer_object
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif
    JAM_LOG("CRenderTargetDepthOGLBase::Unbind() - id: %d\n", m_Id);
}

CRenderTargetStencilPtr CRenderTargetDepthOGLBase::StencilTarget()
{
    return m_Stencil;
}

void CRenderTargetDepthOGLBase::BindToFrameBuffer()
{
#if GL_ARB_framebuffer_object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        m_Id);
#endif
    JAM_LOG("CRenderTargetDepthOGLBase::BindToFrameBuffer() - id: %d\n", m_Id);
}

void CRenderTargetDepthOGLBase::UnbindFromFrameBuffer()
{
#if GL_ARB_framebuffer_object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        0);
#endif
    JAM_LOG("CRenderTargetDepthOGLBase::UnbindFromFrameBuffer() - id: %d\n", m_Id);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CRenderTargetDepthOGLBase::CreateStencil()
{
    assert(!m_Stencil);

    m_Stencil = CreateStencilObject();
    if (m_Stencil) {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(m_Stencil)->InitializeWithDepthId(m_Id);
    }
}

void CRenderTargetDepthOGLBase::DeleteStencil()
{
    assert(m_Stencil);

    std::static_pointer_cast<CRenderTargetStencilOGLBase>(m_Stencil)->InitializeWithDepthId(0);
    m_Stencil = nullptr;
}

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
