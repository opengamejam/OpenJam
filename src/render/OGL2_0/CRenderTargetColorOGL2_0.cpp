//
//  CRenderTargetColorOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRenderTargetColorOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGL2_0::CRenderTargetColorOGL2_0()
: m_Id(0)
, m_InternalFormat(0)
{

}

CRenderTargetColorOGL2_0::~CRenderTargetColorOGL2_0()
{

}

void CRenderTargetColorOGL2_0::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    
    glGenRenderbuffers(1, &m_Id);
    m_InternalFormat = ConvertInternalFormat(internalFormat);
}

void CRenderTargetColorOGL2_0::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetColorOGL2_0::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetColorOGL2_0::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetColorOGL2_0::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetColorOGL2_0::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetColorOGL2_0::BindToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetColorOGL2_0::UnbindFromFrameBuffer(uint64_t colorAttachementIdx)
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

GLenum CRenderTargetColorOGL2_0::ConvertInternalFormat(InternalFormats internalFormat)
{
    return GL_RGBA8_OES; // TODO:
}

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
