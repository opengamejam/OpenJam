//
//  CRenderTargetColorOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CRenderTargetColorOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGL1_5::CRenderTargetColorOGL1_5()
: m_Id(0)
, m_InternalFormat(0)
{

}

CRenderTargetColorOGL1_5::~CRenderTargetColorOGL1_5()
{

}

void CRenderTargetColorOGL1_5::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized())
    {
        return;
    }
    
    glGenRenderbuffers(1, &m_Id);
    m_InternalFormat = ConvertInternalFormat(internalFormat);
}

void CRenderTargetColorOGL1_5::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    
    glDeleteRenderbuffers(1, &m_Id);
}

bool CRenderTargetColorOGL1_5::IsInitialized()
{
    return (m_Id > 0);
}

void CRenderTargetColorOGL1_5::Allocate(uint64_t width, uint64_t height)
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES,
                          static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void CRenderTargetColorOGL1_5::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void CRenderTargetColorOGL1_5::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderTargetColorOGL1_5::BindToFrameBuffer(uint64_t colorAttachementIdx)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0 + colorAttachementIdx,
                              GL_RENDERBUFFER,
                              m_Id);
}

void CRenderTargetColorOGL1_5::UnbindFromFrameBuffer(uint64_t colorAttachementIdx)
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
GLenum CRenderTargetColorOGL1_5::ConvertInternalFormat(InternalFormats internalFormat)
{
    return GL_RGBA8_OES; // TODO:
}

#endif /* defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) */
