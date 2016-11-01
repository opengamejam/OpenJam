//
//  CFrameBufferOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CFrameBufferOGLBase.h"
#include "IRenderTarget.h"
#include "CRenderTargetColorOGLBase.h"
#include "CRenderTargetDepthOGLBase.h"
#include "CRenderTargetStencilOGLBase.h"
#include "CRenderTargetTextureOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGLBase::CFrameBufferOGLBase(uint32_t width, uint32_t height)
: m_FrameBuffer(-1)
, m_NumColorAtachments(1)
, m_Width(width)
, m_Height(height)
, m_ClearColor(CColor4f(0.0f, 0.0f, 1.0f, 1.0f))
{
#if GL_MAX_COLOR_ATTACHMENTS
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_NumColorAtachments);
#endif
    m_ColorBuffers.resize(m_NumColorAtachments);
}

CFrameBufferOGLBase::~CFrameBufferOGLBase()
{
}

void CFrameBufferOGLBase::Initialize()
{
    if (IsInitialized())
    {
        return;
    }
    glGenFramebuffers(1, &m_FrameBuffer);
}

void CFrameBufferOGLBase::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    glDeleteFramebuffers(1, &m_FrameBuffer);
    m_FrameBuffer = -1;
}

bool CFrameBufferOGLBase::IsInitialized()
{
    return (m_FrameBuffer != -1);
}

void CFrameBufferOGLBase::Resize(uint64_t width, uint64_t height)
{
    m_Width = width;
    m_Height = height;
    
    Bind();
    
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [width, height](IRenderTargetPtr renderBuffer)
    {
        if (renderBuffer && renderBuffer->ColorTarget())
        {
            std::static_pointer_cast<CRenderTargetColorOGLBase>(renderBuffer)->Allocate(width, height);
        }
        else if (renderBuffer && renderBuffer->TextureTarget())
        {
            //std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)-> // TODO:
        }
    });
    
    if (DepthAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(DepthAttachement())->Allocate(width, height);
    }
    
    if (StencilAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(StencilAttachement())->Allocate(width, height);
    }
    
    if (IsValid())
    {
        glViewport(0, 0, width, height);
    }
}

uint64_t CFrameBufferOGLBase::MaxColorAttachements() const
{
    return m_NumColorAtachments;
}

void CFrameBufferOGLBase::AttachColor(IRenderTargetPtr colorTarget, uint64_t index)
{
    if (!colorTarget)
    {
        return;
    }
    
    if (index < MaxColorAttachements())
    {
        Bind();
        
        IRenderTargetPtr renderBuffer = colorTarget->ColorTarget();
        IRenderTargetPtr renderTexture = colorTarget->TextureTarget();
        if (renderBuffer)
        {
            std::static_pointer_cast<CRenderTargetColorOGLBase>(renderBuffer)->BindToFrameBuffer(index);
        }
        else if (renderTexture)
        {
            std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsColorToFrameBuffer(index);
        }
        
        m_ColorBuffers[index] = colorTarget;
    }
}

void CFrameBufferOGLBase::AttachDepth(IRenderTargetPtr depthTarget)
{
    if (!depthTarget)
    {
        return;
    }
    
    Bind();
    IRenderTargetPtr renderBuffer = depthTarget->DepthTarget();
    IRenderTargetPtr renderTexture = depthTarget->TextureTarget();
    if (renderBuffer)
    {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(renderBuffer)->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsDepthToFrameBuffer();
    }
    
    m_DepthBuffer = depthTarget;
}

void CFrameBufferOGLBase::AttachStencil(IRenderTargetPtr stencilTarget)
{
    if (!stencilTarget)
    {
        return;
    }
    
    Bind();
    IRenderTargetPtr renderBuffer = stencilTarget->StencilTarget();
    IRenderTargetPtr renderTexture = stencilTarget->TextureTarget();
    if (renderBuffer)
    {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(renderBuffer)->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsStencilToFrameBuffer();
    }
}

void CFrameBufferOGLBase::DetachColor(uint64_t index)
{
    if (index < MaxColorAttachements())
    {
        Bind();
        IRenderTargetPtr renderBuffer = m_ColorBuffers[index];
        if (renderBuffer && renderBuffer->ColorTarget())
        {
            std::static_pointer_cast<CRenderTargetColorOGLBase>(renderBuffer)->UnbindFromFrameBuffer(index);
        }
        else if (renderBuffer && renderBuffer->TextureTarget())
        {
            std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsColorFromFrameBuffer(index);
        }
        m_ColorBuffers[index] = nullptr;
    }
}

void CFrameBufferOGLBase::DetachDepth()
{
    Bind();
    IRenderTargetPtr renderBuffer = m_DepthBuffer;
    if (renderBuffer && renderBuffer->DepthTarget())
    {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(renderBuffer)->UnbindFromFrameBuffer();
    }
    else if (renderBuffer && renderBuffer->TextureTarget())
    {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsDepthFromFrameBuffer();
    }
    m_DepthBuffer = nullptr;
}

void CFrameBufferOGLBase::DetachStencil()
{
    Bind();
    IRenderTargetPtr renderBuffer = m_StencilBuffer;
    if (renderBuffer && renderBuffer->StencilTarget())
    {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(renderBuffer)->UnbindFromFrameBuffer();
    }
    else if (renderBuffer && renderBuffer->TextureTarget())
    {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsStencilFromFrameBuffer();
    }
    m_StencilBuffer = nullptr;
}

IRenderTargetPtr CFrameBufferOGLBase::ColorAttachement(uint64_t index) const
{
    if (index < MaxColorAttachements())
    {
        return m_ColorBuffers[index];
    }
    
    return nullptr;
}

IRenderTargetPtr CFrameBufferOGLBase::DepthAttachement() const
{
    return m_DepthBuffer;
}

IRenderTargetPtr CFrameBufferOGLBase::StencilAttachement() const
{
    return m_StencilBuffer;
}

bool CFrameBufferOGLBase::IsValid() const
{
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    return (result == GL_FRAMEBUFFER_COMPLETE);
}

void CFrameBufferOGLBase::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    
    if (IsValid())
    {
        glViewport(0, 0, Width(), Height());
    }
}

void CFrameBufferOGLBase::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferOGLBase::Clear() const
{
    glClearColor(m_ClearColor.R(), m_ClearColor.G(), m_ClearColor.B(), m_ClearColor.A());
    
    GLbitfield clearBits = GL_COLOR_BUFFER_BIT;
    if (DepthAttachement())
    {
        clearBits |= GL_DEPTH_BUFFER_BIT;
    }
    if (StencilAttachement())
    {
        clearBits |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(clearBits);
}

void CFrameBufferOGLBase::ClearColor(const CColor4f& color)
{
    m_ClearColor = color;
}

const CColor4f& CFrameBufferOGLBase::ClearColor() const
{
    return m_ClearColor;
}

uint32_t CFrameBufferOGLBase::Width() const
{
    return m_Width;
}

uint32_t CFrameBufferOGLBase::Height() const
{
    return m_Height;
}

IFrameBuffer::TRawData CFrameBufferOGLBase::RawData()
{
    Bind();
    
    uint32_t rawdataSize = Width() * Height() * 4;
    IFrameBuffer::TRawData data(rawdataSize, 0);
    
#ifdef GL3_PROTOTYPES // TODO
    glReadPixels(0, 0, Width(), Height(), GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
#endif
    
    Unbind();
    
    return data;
}

void CFrameBufferOGLBase::InitializeWithFBO(GLint fbo)
{
    m_FrameBuffer = fbo;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */