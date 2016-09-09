//
//  CRenderTargetColorOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#include "CRenderTargetColorOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGL1_3::CRenderTargetColorOGL1_3()
: m_Id(0)
{

}

CRenderTargetColorOGL1_3::~CRenderTargetColorOGL1_3()
{

}

void CRenderTargetColorOGL1_3::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetColorOGL1_3::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetColorOGL1_3::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetColorOGL1_3::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width, height);
}

void CRenderTargetColorOGL1_3::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetColorOGL1_3::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetColorOGL1_3::BindToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetColorOGL1_3::UnbindFromFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
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