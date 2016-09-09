//
//  CRenderTargetDepthOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#include "CRenderTargetDepthOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL1_3::CRenderTargetDepthOGL1_3()
: m_Id(0)
{

}

CRenderTargetDepthOGL1_3::~CRenderTargetDepthOGL1_3()
{

}

void CRenderTargetDepthOGL1_3::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetDepthOGL1_3::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetDepthOGL1_3::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGL1_3::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
}

void CRenderTargetDepthOGL1_3::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetDepthOGL1_3::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetDepthOGL1_3::BindToFrameBuffer()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetDepthOGL1_3::UnbindFromFrameBuffer()
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

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) */