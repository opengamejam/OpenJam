//
//  CRenderTargetDepthOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetDepthOGL2_0.h"
#include "CRenderTargetStencilOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL2_0::CRenderTargetDepthOGL2_0()
: m_Id(0)
, m_InternalFormat(0)
, m_Stencil(nullptr)
{

}

CRenderTargetDepthOGL2_0::~CRenderTargetDepthOGL2_0()
{
    DeleteStencil();
}

void CRenderTargetDepthOGL2_0::Initialize(InternalFormats internalFormat)
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

void CRenderTargetDepthOGL2_0::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetDepthOGL2_0::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGL2_0::Allocate(uint64_t width, uint64_t height)
{    
    glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetDepthOGL2_0::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetDepthOGL2_0::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

CRenderTargetStencilPtr CRenderTargetDepthOGL2_0::StencilTarget()
{
    return m_Stencil;
}

void CRenderTargetDepthOGL2_0::BindToFrameBuffer()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetDepthOGL2_0::UnbindFromFrameBuffer()
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

void CRenderTargetDepthOGL2_0::CreateStencil()
{
    if (m_Stencil)
    {
        return;
    }
    
    m_Stencil.reset(new CRenderTargetStencilOGL2_0());
    std::static_pointer_cast<CRenderTargetStencilOGL2_0>(m_Stencil)->InitializeWithDepthId(m_Id);
}

void CRenderTargetDepthOGL2_0::DeleteStencil()
{
    if (!m_Stencil)
    {
        return;
    }
    
    std::static_pointer_cast<CRenderTargetStencilOGL2_0>(m_Stencil)->InitializeWithDepthId(0);
    m_Stencil = nullptr;
}

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
