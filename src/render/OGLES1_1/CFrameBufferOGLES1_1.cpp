//
//  CFrameBufferOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#include "CFrameBufferOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGLES1_1::CFrameBufferOGLES1_1(uint32_t width, uint32_t height)
: m_FrameBuffer(-1)
, m_DepthBuffer(-1)
, m_StencilBuffer(-1)
, m_NumColorAtachments(1)
, m_IsFrameBufferExt(false)
, m_IsColor0BufferExt(false)
, m_IsDepthBufferExt(false)
, m_IsStencilBufferExt(false)
, m_Width(width)
, m_Height(height)
, m_ClearColor(CColor4f(0.0f, 0.0f, 1.0f, 1.0f))
{
#if GL_MAX_COLOR_ATTACHMENTS
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_NumColorAtachments);
#endif
    m_ColorBuffers.resize(m_NumColorAtachments);
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](uint32_t& colorBuffer)
    {
        colorBuffer = -1;
    });
}

CFrameBufferOGLES1_1::~CFrameBufferOGLES1_1()
{
    uint32_t i = 0;
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](uint32_t colorBuffer)
    {
        if (colorBuffer != -1 && (i != 0 || !m_IsColor0BufferExt))
        {
            glDeleteRenderbuffers(1, &colorBuffer);
        }
    });
    
    if (m_DepthBuffer != -1)
    {
#if GL_OES_packed_depth_stencil
        m_StencilBuffer = -1;
#endif
        glDeleteRenderbuffersOES(1, &m_DepthBuffer);
    }
    
    if (m_StencilBuffer != -1)
    {
        glDeleteRenderbuffersOES(1, &m_StencilBuffer);
    }
    
    if (m_FrameBuffer != -1 && !m_IsDepthBufferExt)
    {
        glDeleteFramebuffersOES(1, &m_FrameBuffer);
    }
    
    assert(glGetError() == GL_NO_ERROR);
}

void CFrameBufferOGLES1_1::Initialize(uint32_t externalFrameBuffer, uint32_t externalColorBuffer,
                                          uint32_t externalDepthBuffer, uint32_t externalStencilBuffer)
{
    if (m_FrameBuffer == -1 && externalFrameBuffer != -1)
    {
        m_FrameBuffer = externalFrameBuffer;
        m_IsFrameBufferExt = true;
    }
    
    if (m_ColorBuffers[0] == -1 && externalColorBuffer != -1)
    {
        m_ColorBuffers[0] = externalColorBuffer;
        m_IsColor0BufferExt = true;
    }
    
    if (m_DepthBuffer == -1 && externalDepthBuffer != -1)
    {
        m_DepthBuffer = externalDepthBuffer;
        m_IsDepthBufferExt = true;
    }
    
    if (m_StencilBuffer == -1 && externalStencilBuffer != -1)
    {
        m_StencilBuffer = externalStencilBuffer;
        m_IsStencilBufferExt = true;
    }
    
    Initialize();
}

void CFrameBufferOGLES1_1::Initialize()
{
    if (m_FrameBuffer == -1)
    {
        glGenFramebuffersOES(1, &m_FrameBuffer);
    }
    glBindFramebufferOES(GL_FRAMEBUFFER, m_FrameBuffer);
    
    assert(glGetError() == GL_NO_ERROR);
}

bool CFrameBufferOGLES1_1::CreateColorAttachment(int index)
{
    if (index >= m_ColorBuffers.size())
    {
        return false;
    }
    
    if (m_ColorBuffers[index] != -1)
    {
        return true;
    }
    
    glGenRenderbuffers(1, &m_ColorBuffers[index]);
    glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffers[index]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, Width(), Height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, m_ColorBuffers[index]);
    
    bool result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    if (result)
    {
        glViewport(0, 0, Width(), Height());
    }
    
    assert(glGetError() == GL_NO_ERROR);
    return result;
}

bool CFrameBufferOGLES1_1::CreateDepthAttachment()
{
    if (m_DepthBuffer != -1)
    {
        return true;
    }
    
    glGenRenderbuffers(1, &m_DepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
    
#if GL_OES_packed_depth_stencil
    m_StencilBuffer = m_DepthBuffer;
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, Width(), Height());
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_StencilBuffer);
#else
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width(), Height());
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
#endif
    
    assert(glGetError() == GL_NO_ERROR);
    bool result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    return result;
}

bool CFrameBufferOGLES1_1::CreateStencilAttachment()
{
    if (m_StencilBuffer != -1)
    {
        return true;
    }
    
    bool result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    return result;
}

bool CFrameBufferOGLES1_1::CreateTextureAttachment()
{
    return false;   // TODO:
}

void CFrameBufferOGLES1_1::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void CFrameBufferOGLES1_1::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferOGLES1_1::Clear() const
{
    glClearColor(m_ClearColor.R(), m_ClearColor.G(), m_ClearColor.B(), m_ClearColor.A());
    
    GLbitfield clearBits = 0;
    if (true) // TODO: checks for s, that has only depth or stencil render buffers
    {
        clearBits |= GL_COLOR_BUFFER_BIT;
    }
    if (m_DepthBuffer != 0)
    {
        clearBits |= GL_DEPTH_BUFFER_BIT;
    }
    if (m_StencilBuffer != 0)
    {
        clearBits |= GL_STENCIL_BUFFER_BIT;
    }
    if (clearBits)
    {
        glClear(clearBits);
    }
}

void CFrameBufferOGLES1_1::ClearColor(const CColor4f& color)
{
    m_ClearColor = color;
}

const CColor4f& CFrameBufferOGLES1_1::ClearColor() const
{
    return m_ClearColor;
}

uint32_t CFrameBufferOGLES1_1::Width() const
{
    return m_Width;
}

uint32_t CFrameBufferOGLES1_1::Height() const
{
    return m_Height;
}

IRenderTarget::TRawData CFrameBufferOGLES1_1::RawData()
{
    Bind();
    
    uint32_t rawdataSize = Width() * Height() * 4;
    IRenderTarget::TRawData data(rawdataSize, 0);
    glReadPixels(0, 0, Width(), Height(), GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    
    Unbind();
    
    return data;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1_1
