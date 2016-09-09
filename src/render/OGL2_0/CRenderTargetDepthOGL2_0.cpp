//
//  CRenderTargetDepthOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetDepthOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL2_0::CRenderTargetDepthOGL2_0()
: m_Id(0)
{

}

CRenderTargetDepthOGL2_0::~CRenderTargetDepthOGL2_0()
{

}

void CRenderTargetDepthOGL2_0::Initialize()
{
    if (!IsInitialized())
    {
        glGenRenderbuffers(1, &m_Id);
    }
}

void CRenderTargetDepthOGL2_0::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteRenderbuffers(1, &m_Id);
    }
}

bool CRenderTargetDepthOGL2_0::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetDepthOGL2_0::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
}

void CRenderTargetDepthOGL2_0::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetDepthOGL2_0::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)