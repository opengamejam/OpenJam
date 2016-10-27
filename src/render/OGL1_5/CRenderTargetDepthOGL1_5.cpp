//
//  CRenderTargetDepthOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CRenderTargetDepthOGL1_5.h"
#include "CRenderTargetStencilOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL1_5::CRenderTargetDepthOGL1_5()
: m_Id(0)
, m_InternalFormat(0)
, m_Stencil(nullptr)
{

}

CRenderTargetDepthOGL1_5::~CRenderTargetDepthOGL1_5()
{

}

void CRenderTargetDepthOGL1_5::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    glGenRenderbuffers(1, &m_Id);
    
    switch (internalFormat & 0xFFFFFFE0) {
        case Depth16:
            m_InternalFormat = GL_DEPTH_COMPONENT16_OES;
            DeleteStencil();
            break;
        case Depth24:
            m_InternalFormat = GL_DEPTH_COMPONENT24_OES;
            DeleteStencil();
            break;
        case Depth32:
            m_InternalFormat = GL_DEPTH_COMPONENT32_OES;
            DeleteStencil();
            break;
#if GL_OES_packed_depth_stencil
        case Depth24_Stencil8:
            m_InternalFormat = GL_DEPTH24_STENCIL8_OES;
            CreateStencil();
            break;
#endif
        default:
            m_InternalFormat = GL_DEPTH_COMPONENT24_OES;
            DeleteStencil();
            break;
    }
}

void CRenderTargetDepthOGL1_5::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetDepthOGL1_5::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGL1_5::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetDepthOGL1_5::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetDepthOGL1_5::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetDepthOGL1_5::BindToFrameBuffer()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetDepthOGL1_5::UnbindFromFrameBuffer()
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

void CRenderTargetDepthOGL1_5::CreateStencil()
{
    if (m_Stencil)
    {
        return;
    }
    
    m_Stencil.reset(new CRenderTargetStencilOGL1_5());
    std::static_pointer_cast<CRenderTargetStencilOGL1_5>(m_Stencil)->InitializeWithDepthId(m_Id);
}

void CRenderTargetDepthOGL1_5::DeleteStencil()
{
    if (!m_Stencil)
    {
        return;
    }
    
    std::static_pointer_cast<CRenderTargetStencilOGL1_5>(m_Stencil)->InitializeWithDepthId(0);
    m_Stencil = nullptr;
}

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
