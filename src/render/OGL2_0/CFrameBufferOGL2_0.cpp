//
//  CFrameBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CFrameBufferOGL2_0.h"
#include "IRenderTarget.h"
#include "CRenderTargetColorOGL2_0.h"
#include "CRenderTargetDepthOGL2_0.h"
#include "CRenderTargetStencilOGL2_0.h"
#include "CRenderTargetTextureOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGL2_0::CFrameBufferOGL2_0(uint32_t width, uint32_t height)
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

CFrameBufferOGL2_0::~CFrameBufferOGL2_0()
{
    Shutdown();
}

void CFrameBufferOGL2_0::Initialize()
{
    if (!IsInitialized())
    {
        glGenFramebuffers(1, &m_FrameBuffer);
    }
}

void CFrameBufferOGL2_0::Shutdown()
{
    if (IsInitialized())
    {
        glDeleteFramebuffers(1, &m_FrameBuffer);
        m_FrameBuffer = -1;
    }
}

bool CFrameBufferOGL2_0::IsInitialized()
{
    return (m_FrameBuffer != -1);
}

void CFrameBufferOGL2_0::Resize(uint64_t width, uint64_t height)
{
    m_Width = width;
    m_Height = height;
    
    Bind();
    
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [width, height](IRenderTargetPtr renderBuffer)
    {
        if (renderBuffer && renderBuffer->ColorTarget())
        {
            std::static_pointer_cast<CRenderTargetColorOGL2_0>(renderBuffer)->Allocate(width, height);
        }
        else if (renderBuffer && renderBuffer->TextureTarget())
        {
            //std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderBuffer)-> // TODO:
        }
    });
    
    if (DepthAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGL2_0>(DepthAttachement())->Allocate(width, height);
    }
    
    if (StencilAttachement())
    {
        std::static_pointer_cast<CRenderTargetDepthOGL2_0>(StencilAttachement())->Allocate(width, height);
    }
    
    if (IsValid())
    {
        glViewport(0, 0, width, height);
    }
}

uint64_t CFrameBufferOGL2_0::MaxColorAttachements() const
{
    return m_NumColorAtachments;
}

void CFrameBufferOGL2_0::AttachColor(IRenderTargetPtr colorTarget, uint64_t index)
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
            std::static_pointer_cast<CRenderTargetColorOGL2_0>(renderBuffer)->BindToFrameBuffer(index);
        }
        else if (renderTexture)
        {
            std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderTexture)->BindAsColorToFrameBuffer(index);
        }
        
        m_ColorBuffers[index] = colorTarget;
    }
}

void CFrameBufferOGL2_0::AttachDepth(IRenderTargetPtr depthTarget)
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
        std::static_pointer_cast<CRenderTargetDepthOGL2_0>(renderBuffer)->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderTexture)->BindAsDepthToFrameBuffer();
    }
    
    m_DepthBuffer = depthTarget;
}

void CFrameBufferOGL2_0::AttachStencil(IRenderTargetPtr stencilTarget)
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
        std::static_pointer_cast<CRenderTargetStencilOGL2_0>(renderBuffer)->BindToFrameBuffer();
    }
    else if (renderTexture)
    {
        std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderTexture)->BindAsStencilToFrameBuffer();
    }
}

void CFrameBufferOGL2_0::DetachColor(uint64_t index)
{
    if (index < MaxColorAttachements())
    {
        Bind();
        IRenderTargetPtr renderBuffer = m_ColorBuffers[index];
        if (renderBuffer && renderBuffer->ColorTarget())
        {
            std::static_pointer_cast<CRenderTargetColorOGL2_0>(renderBuffer)->UnbindFromFrameBuffer(index);
        }
        else if (renderBuffer && renderBuffer->TextureTarget())
        {
            std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderBuffer)->UnbindAsColorFromFrameBuffer(index);
        }
        m_ColorBuffers[index] = nullptr;
    }
}

void CFrameBufferOGL2_0::DetachDepth()
{
    Bind();
    IRenderTargetPtr renderBuffer = m_DepthBuffer;
    if (renderBuffer && renderBuffer->DepthTarget())
    {
        std::static_pointer_cast<CRenderTargetDepthOGL2_0>(renderBuffer)->UnbindFromFrameBuffer();
    }
    else if (renderBuffer && renderBuffer->TextureTarget())
    {
        std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderBuffer)->UnbindAsDepthFromFrameBuffer();
    }
    m_DepthBuffer = nullptr;
}

void CFrameBufferOGL2_0::DetachStencil()
{
    Bind();
    IRenderTargetPtr renderBuffer = m_StencilBuffer;
    if (renderBuffer && renderBuffer->StencilTarget())
    {
        std::static_pointer_cast<CRenderTargetStencilOGL2_0>(renderBuffer)->UnbindFromFrameBuffer();
    }
    else if (renderBuffer && renderBuffer->TextureTarget())
    {
        std::static_pointer_cast<CRenderTargetTextureOGL2_0>(renderBuffer)->UnbindAsStencilFromFrameBuffer();
    }
    m_StencilBuffer = nullptr;
}

IRenderTargetPtr CFrameBufferOGL2_0::ColorAttachement(uint64_t index) const
{
    if (index < MaxColorAttachements())
    {
        return m_ColorBuffers[index];
    }
    
    return nullptr;
}

IRenderTargetPtr CFrameBufferOGL2_0::DepthAttachement() const
{
    return m_DepthBuffer;
}

IRenderTargetPtr CFrameBufferOGL2_0::StencilAttachement() const
{
    return m_StencilBuffer;
}

bool CFrameBufferOGL2_0::IsValid() const
{
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    return (result == GL_FRAMEBUFFER_COMPLETE);
}

void CFrameBufferOGL2_0::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    
    if (IsValid())
    {
        glViewport(0, 0, Width(), Height());
    }
}

void CFrameBufferOGL2_0::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBufferOGL2_0::Clear() const
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

void CFrameBufferOGL2_0::ClearColor(const CColor4f& color)
{
    m_ClearColor = color;
}

const CColor4f& CFrameBufferOGL2_0::ClearColor() const
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

IFrameBuffer::TRawData CFrameBufferOGL2_0::RawData()
{
    Bind();
    
    uint32_t rawdataSize = Width() * Height() * 4;
    IFrameBuffer::TRawData data(rawdataSize, 0);
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

#endif // defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)
