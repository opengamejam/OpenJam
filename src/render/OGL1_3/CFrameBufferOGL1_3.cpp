//
//  CFrameBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#include "CFrameBufferOGL1_3.h"
#include "IRenderTarget.h"
#include "CRenderTargetColorOGL1_3.h"
#include "CRenderTargetDepthOGL1_3.h"
#include "CRenderTargetStencilOGL1_3.h"
#include "CRenderTargetTextureOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGL1_3::CFrameBufferOGL1_3(uint32_t width, uint32_t height)
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

CFrameBufferOGL1_3::~CFrameBufferOGL1_3()
{
    Shutdown();
}

void CFrameBufferOGL1_3::Initialize()
{
    if (!IsInitialized())
    {
        glGenFramebuffers(1, &m_FrameBuffer);
    }
}

void CFrameBufferOGL1_3::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteFramebuffers(1, &m_FrameBuffer);
        m_FrameBuffer = -1;
    }
}

bool CFrameBufferOGL1_3::IsInitialized()
{
    return (m_FrameBuffer > -1);
}

void CFrameBufferOGL1_3::Resize(uint64_t width, uint64_t height)
{
    m_Width = width;
    m_Height = height;
    
    Bind();
    
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [width, height](IRenderTargetPtr renderBuffer)
    {
        if (renderBuffer && renderBuffer->ColorTarget())
        {
            std::static_pointer_cast<CRenderTargetColorOGL1_3>(renderBuffer)->Allocate(width, height);
        }
        else if (renderBuffer && renderBuffer->TextureTarget())
        {
            //std::static_pointer_cast<CRenderTargetTextureOGL1_3>(renderBuffer)-> // TODO:
        }
    });
    
    if (DepthAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGL1_3>(DepthAttachement())->Allocate(width, height);
    }
    
    if (StencilAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGL1_3>(StencilAttachement())->Allocate(width, height);
    }
    
    if (IsValid())
    {
        glViewport(0, 0, width, height);
    }
}

uint64_t CFrameBufferOGL1_3::MaxColorAttachements() const
{
    return m_NumColorAtachments;
}

void CFrameBufferOGL1_3::AttachColor(IRenderTargetPtr colorTarget, uint64_t index)
{
    if (!colorTarget)
    {
        return;
    }
    
    if (index < MaxColorAttachements())
    {
        Bind();
        
        std::shared_ptr<CRenderTargetColorOGL1_3> renderBuffer = std::static_pointer_cast<CRenderTargetColorOGL1_3>(colorTarget->ColorTarget());
        std::shared_ptr<CRenderTargetTextureOGL1_3> renderTexture = std::static_pointer_cast<CRenderTargetTextureOGL1_3>(colorTarget->TextureTarget());
        if (renderBuffer)
        {
            renderBuffer->BindToFrameBuffer(index);
        }
        else if (renderTexture)
        {
            renderTexture->BindAsColorToFrameBuffer(index);
        }
        
        m_ColorBuffers[index] = colorTarget;
    }
}

void CFrameBufferOGL1_3::AttachDepth(IRenderTargetPtr depthTarget)
{
    if (!depthTarget)
    {
        return;
    }
    
    Bind();
    std::shared_ptr<CRenderTargetDepthOGL1_3> renderBuffer = std::static_pointer_cast<CRenderTargetDepthOGL1_3>(depthTarget->DepthTarget());
    std::shared_ptr<CRenderTargetTextureOGL1_3> renderTexture = std::static_pointer_cast<CRenderTargetTextureOGL1_3>(depthTarget->TextureTarget());
    if (renderBuffer)
    {
        renderBuffer->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        renderTexture->BindAsDepthToFrameBuffer();
    }
    
    m_DepthBuffer = depthTarget;
}

void CFrameBufferOGL1_3::AttachStencil(IRenderTargetPtr stencilTarget)
{
    if (!stencilTarget)
    {
        return;
    }
    
    Bind();
    std::shared_ptr<CRenderTargetStencilOGL1_3> renderBuffer = std::static_pointer_cast<CRenderTargetStencilOGL1_3>(stencilTarget->StencilTarget());
    std::shared_ptr<CRenderTargetTextureOGL1_3> renderTexture = std::static_pointer_cast<CRenderTargetTextureOGL1_3>(stencilTarget->TextureTarget());
    if (renderBuffer)
    {
        renderBuffer->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        renderTexture->BindAsStencilToFrameBuffer();
    }
}

void CFrameBufferOGL1_3::DetachColor(uint64_t index)
{
    if (index < MaxColorAttachements())
    {
        Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, 0);
        m_ColorBuffers[index] = nullptr;
    }
}

void CFrameBufferOGL1_3::DetachDepth()
{
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    m_DepthBuffer = nullptr;
}

void CFrameBufferOGL1_3::DetachStencil()
{
    Bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    m_StencilBuffer = nullptr;
}

IRenderTargetPtr CFrameBufferOGL1_3::ColorAttachement(uint64_t index) const
{
    if (index < MaxColorAttachements())
    {
        return m_ColorBuffers[index];
    }
    
    return nullptr;
}

IRenderTargetPtr CFrameBufferOGL1_3::DepthAttachement() const
{
    return m_DepthBuffer;
}

IRenderTargetPtr CFrameBufferOGL1_3::StencilAttachement() const
{
    return m_StencilBuffer;
}

bool CFrameBufferOGL1_3::IsValid() const
{
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void CFrameBufferOGL1_3::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    
    if (IsValid())
    {
        glViewport(0, 0, Width(), Height());
    }
}

void CFrameBufferOGL1_3::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferOGL1_3::Clear() const
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

void CFrameBufferOGL1_3::ClearColor(const CColor4f& color)
{
    m_ClearColor = color;
}

const CColor4f& CFrameBufferOGL1_3::ClearColor() const
{
    return m_ClearColor;
}

uint32_t CFrameBufferOGL1_3::Width() const
{
    return m_Width;
}

uint32_t CFrameBufferOGL1_3::Height() const
{
    return m_Height;
}

IFrameBuffer::TRawData CFrameBufferOGL1_3::RawData()
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

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3
