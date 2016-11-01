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

    glGenRenderbuffers(1, &m_Id);
}

void CRenderTargetDepthOGLBase::Shutdown()
{
    assert(IsInitialized());
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetDepthOGLBase::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGLBase::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, ConvertToInternalFormat(m_InternalFormat),
        static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetDepthOGLBase::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetDepthOGLBase::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

CRenderTargetStencilPtr CRenderTargetDepthOGLBase::StencilTarget()
{
    return m_Stencil;
}

void CRenderTargetDepthOGLBase::BindToFrameBuffer()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        m_Id);
}

void CRenderTargetDepthOGLBase::UnbindFromFrameBuffer()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        0);
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
