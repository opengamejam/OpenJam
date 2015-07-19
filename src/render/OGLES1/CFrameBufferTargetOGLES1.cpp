//
//  CFrameBufferRenderTargetOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CFrameBufferTargetOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

typedef void(*XglGenFramebuffers)(GLsizei n, GLuint* framebuffers);
typedef void(*XglBindFramebuffer)(GLenum target, GLuint framebuffer);
typedef void(*XglDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
typedef void(*XglDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
typedef void(*XglFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void(*XglFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef GLenum(*XglCheckFramebufferStatus)(GLenum target);
typedef void(*XglGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
typedef void(*XglBindRenderbuffer)(GLenum target, GLuint renderbuffer);
typedef void(*XglRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

#if defined(OS_IPHONE)

static XglGenFramebuffers xglGenFramebuffers = &glGenFramebuffersOES;
static XglBindFramebuffer xglBindFramebuffer = &glBindFramebufferOES;
static XglDeleteFramebuffers xglDeleteFramebuffers = &glDeleteFramebuffersOES;
//static XglFramebufferTexture2D xglFramebufferTexture2D = &glFramebufferTexture2DOES;
static XglFramebufferRenderbuffer xglFramebufferRenderbuffer = &glFramebufferRenderbufferOES;
static XglCheckFramebufferStatus xglCheckFramebufferStatus = &glCheckFramebufferStatusOES;
static XglGenRenderbuffers xglGenRenderbuffers = &glGenRenderbuffersOES;
static XglBindRenderbuffer xglBindRenderbuffer = &glBindRenderbufferOES;
static XglDeleteRenderbuffers xglDeleteRenderbuffers = &glDeleteRenderbuffersOES;
static XglRenderbufferStorage xglRenderbufferStorage = &glRenderbufferStorageOES;

#define XGL_FRAMEBUFFER             GL_FRAMEBUFFER_OES
#define XGL_RENDERBUFFER            GL_RENDERBUFFER_OES
#define XGL_COLOR_ATTACHMENT0       GL_COLOR_ATTACHMENT0_OES
#define XGL_DEPTH_ATTACHMENT        GL_DEPTH_ATTACHMENT_OES
#define XGL_STENCIL_ATTACHMENT      GL_STENCIL_ATTACHMENT_OES
#define XGL_FRAMEBUFFER_COMPLETE    GL_FRAMEBUFFER_COMPLETE_OES
#define XGL_DEPTH24_STENCIL8        GL_DEPTH24_STENCIL8_OES

#define XGL_RGBA8                   GL_RGBA8_OES

#else // OS_IPHONE

static XglGenFramebuffers xglGenFramebuffers = &glGenFramebuffers;
static XglBindFramebuffer xglBindFramebuffer = &glBindFramebuffer;
static XglDeleteFramebuffers xglDeleteFramebuffers = &glDeleteFramebuffers;
//static XglFramebufferTexture2D xglFramebufferTexture2D = &glFramebufferTexture2D;
static XglFramebufferRenderbuffer xglFramebufferRenderbuffer = &glFramebufferRenderbuffer;
static XglCheckFramebufferStatus xglCheckFramebufferStatus = &glCheckFramebufferStatus;
static XglGenRenderbuffers xglGenRenderbuffers = &glGenRenderbuffers;
static XglBindRenderbuffer xglBindRenderbuffer = &glBindRenderbuffer;
static XglDeleteRenderbuffers xglDeleteRenderbuffers = &glDeleteRenderbuffers;
static XglRenderbufferStorage xglRenderbufferStorage = &glRenderbufferStorage;

#define XGL_FRAMEBUFFER             GL_FRAMEBUFFER
#define XGL_RENDERBUFFER            GL_RENDERBUFFER
#define XGL_COLOR_ATTACHMENT0       GL_COLOR_ATTACHMENT0
#define XGL_DEPTH_ATTACHMENT        GL_DEPTH_ATTACHMENT
#define XGL_STENCIL_ATTACHMENT      GL_STENCIL_ATTACHMENT
#define XGL_FRAMEBUFFER_COMPLETE    GL_FRAMEBUFFER_COMPLETE
#define XGL_DEPTH24_STENCIL8        GL_DEPTH24_STENCIL8

#define XGL_RGBA8                   GL_RGBA8

#endif // OS_IPHONE

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferTargetOGLES1::CFrameBufferTargetOGLES1(unsigned int width, unsigned int height)
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
{
#if GL_MAX_COLOR_ATTACHMENTS
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_NumColorAtachments);
#endif
    m_ColorBuffers.resize(m_NumColorAtachments);
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](unsigned int& colorBuffer)
    {
        colorBuffer = -1;
    });
}

CFrameBufferTargetOGLES1::~CFrameBufferTargetOGLES1()
{
    unsigned int i = 0;
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](unsigned int colorBuffer)
    {
        if (colorBuffer != -1 && (i != 0 || !m_IsColor0BufferExt))
        {
            xglDeleteRenderbuffers(1, &colorBuffer);
        }
    });
    
    if (m_DepthBuffer != -1)
    {
#if GL_OES_packed_depth_stencil
        m_StencilBuffer = -1;
#endif
        xglDeleteRenderbuffers(1, &m_DepthBuffer);
    }
    
    if (m_StencilBuffer != -1)
    {
        xglDeleteRenderbuffers(1, &m_StencilBuffer);
    }
    
    if (m_FrameBuffer != -1 && !m_IsDepthBufferExt)
    {
        xglDeleteFramebuffers(1, &m_FrameBuffer);
    }
}

void CFrameBufferTargetOGLES1::Initialize(unsigned int externalFrameBuffer, unsigned int externalColorBuffer,
                                          unsigned int externalDepthBuffer, unsigned int externalStencilBuffer)
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

void CFrameBufferTargetOGLES1::Initialize()
{
    if (m_FrameBuffer == -1)
    {
        xglGenFramebuffers(1, &m_FrameBuffer);
    }
    xglBindFramebuffer(XGL_FRAMEBUFFER, m_FrameBuffer);
}

bool CFrameBufferTargetOGLES1::CreateColorAttachment(int index)
{
    if (index >= m_ColorBuffers.size())
    {
        return false;
    }
    
    if (m_ColorBuffers[index] != -1)
    {
        return true;
    }
    
    xglGenRenderbuffers(1, &m_ColorBuffers[index]);
    xglBindRenderbuffer(XGL_RENDERBUFFER, m_ColorBuffers[index]);
    xglRenderbufferStorage(XGL_RENDERBUFFER, XGL_RGBA8, Width(), Height());
    xglFramebufferRenderbuffer(XGL_FRAMEBUFFER, XGL_COLOR_ATTACHMENT0 + index, XGL_RENDERBUFFER, m_ColorBuffers[index]);
    
    bool result = (xglCheckFramebufferStatus(XGL_FRAMEBUFFER) == XGL_FRAMEBUFFER_COMPLETE);
    if (result)
    {
        glViewport(0, 0, Width(), Height());
    }
    return result;
}

bool CFrameBufferTargetOGLES1::CreateDepthAttachment()
{
    if (m_DepthBuffer != -1)
    {
        return true;
    }
    
    xglGenRenderbuffers(1, &m_DepthBuffer);
    xglBindRenderbuffer(XGL_RENDERBUFFER, m_DepthBuffer);
    
#if GL_OES_packed_depth_stencil
    m_StencilBuffer = m_DepthBuffer;
    xglRenderbufferStorage(XGL_RENDERBUFFER, XGL_DEPTH24_STENCIL8, Width(), Height());
    
    xglBindFramebuffer(XGL_FRAMEBUFFER, m_FrameBuffer);
    xglFramebufferRenderbuffer(XGL_FRAMEBUFFER, XGL_DEPTH_ATTACHMENT, XGL_RENDERBUFFER, m_DepthBuffer);
    xglFramebufferRenderbuffer(XGL_FRAMEBUFFER, XGL_STENCIL_ATTACHMENT, XGL_RENDERBUFFER, m_StencilBuffer);
#else
    xglRenderbufferStorage(XGL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width(), Height());
    
    xglBindFramebuffer(XGL_FRAMEBUFFER, m_FrameBuffer);
    xglFramebufferRenderbuffer(XGL_FRAMEBUFFER, XGL_DEPTH_ATTACHMENT, XGL_RENDERBUFFER, m_DepthBuffer);
#endif
    
    bool result = (xglCheckFramebufferStatus(XGL_FRAMEBUFFER) == XGL_FRAMEBUFFER_COMPLETE);
    return result;
}

bool CFrameBufferTargetOGLES1::CreateStencilAttachment()
{
    if (m_StencilBuffer != -1)
    {
        return true;
    }
    
    bool result = (xglCheckFramebufferStatus(XGL_FRAMEBUFFER) == XGL_FRAMEBUFFER_COMPLETE);
    return result;
}

bool CFrameBufferTargetOGLES1::CreateTextureAttachment()
{
    return false;   // TODO:
}

void CFrameBufferTargetOGLES1::Bind() const
{
    xglBindFramebuffer(XGL_FRAMEBUFFER, m_FrameBuffer);
}

void CFrameBufferTargetOGLES1::Unbind() const
{
    xglBindFramebuffer(XGL_FRAMEBUFFER, 0);
}

void CFrameBufferTargetOGLES1::Clear() const
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

void CFrameBufferTargetOGLES1::ClearColor(const CColor& color)
{
    m_ClearColor = color;
}

const CColor& CFrameBufferTargetOGLES1::ClearColor() const
{
    return m_ClearColor;
}

unsigned int CFrameBufferTargetOGLES1::Width() const
{
    return m_Width;
}

unsigned int CFrameBufferTargetOGLES1::Height() const
{
    return m_Height;
}

std::vector<unsigned char> CFrameBufferTargetOGLES1::RawData()
{
    Bind();
    
    unsigned int rawdataSize = Width() * Height() * 4;
    std::vector<unsigned char> data(rawdataSize);
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

#endif // RENDER_OGLES1