//
//  CRenderTargetDepthOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CRenderTargetDepthOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL1_5::CRenderTargetDepthOGL1_5()
: m_Id(0)
{

}

CRenderTargetDepthOGL1_5::~CRenderTargetDepthOGL1_5()
{

}

void CRenderTargetDepthOGL1_5::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetDepthOGL1_5::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetDepthOGL1_5::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGL1_5::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
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

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */