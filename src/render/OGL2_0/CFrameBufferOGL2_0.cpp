//
//  CFrameBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CFrameBufferOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGL2_0::CFrameBufferOGL2_0(uint32_t width, uint32_t height)
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
, m_ClearColor(CColor(0.0f, 0.0f, 1.0f, 1.0f))
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

CFrameBufferOGL2_0::~CFrameBufferOGL2_0()
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
        glDeleteRenderbuffers(1, &m_DepthBuffer);
    }
    
    if (m_StencilBuffer != -1)
    {
        glDeleteRenderbuffers(1, &m_StencilBuffer);
    }
    
    if (m_FrameBuffer != -1 && !m_IsDepthBufferExt)
    {
        glDeleteFramebuffers(1, &m_FrameBuffer);
    }
}

void CFrameBufferOGL2_0::Initialize(uint32_t externalFrameBuffer, uint32_t externalColorBuffer,
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

void CFrameBufferOGL2_0::Initialize()
{
    if (m_FrameBuffer == -1)
    {
        glGenFramebuffers(1, &m_FrameBuffer);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

bool CFrameBufferOGL2_0::CreateColorAttachment(int index)
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
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width(), Height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, m_ColorBuffers[index]);
    
    bool result = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    if (result)
    {
        glViewport(0, 0, Width(), Height());
    }
    return result;
}

bool CFrameBufferOGL2_0::CreateDepthAttachment()
{
    if (m_DepthBuffer != -1)
    {
        return true;
    }
    
    glGenRenderbuffers(1, &m_DepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width(), Height());
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
    
    return m_DepthBuffer != -1;
}

bool CFrameBufferOGL2_0::CreateStencilAttachment()
{
    if (m_StencilBuffer != -1)
    {
        return true;
    }
    
    return false; // TODO:
}

bool CFrameBufferOGL2_0::CreateTextureAttachment()
{
    return false;   // TODO:
}

void CFrameBufferOGL2_0::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void CFrameBufferOGL2_0::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, -1);
}

void CFrameBufferOGL2_0::Clear() const
{
    glClearColor(m_ClearColor.R(), m_ClearColor.G(), m_ClearColor.B(), m_ClearColor.A());
    
    GLbitfield clearBits = 0;
    if (true) // TODO: checks for targets, that has only depth or stencil render buffers
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

void CFrameBufferOGL2_0::ClearColor(const CColor& color)
{
    m_ClearColor = color;
}

const CColor& CFrameBufferOGL2_0::ClearColor() const
{
    return m_ClearColor;
}

uint32_t CFrameBufferOGL2_0::Width() const
{
    return m_Width;
}

uint32_t CFrameBufferOGL2_0::Height() const
{
    return m_Height;
}

IRenderTarget::TRawData CFrameBufferOGL2_0::RawData()
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

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0
