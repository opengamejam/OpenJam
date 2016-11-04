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
    : m_Id(-1)
    , m_NumColorAtachments(1)
    , m_Width(width)
    , m_Height(height)
    , m_ClearColor(CColor4f(0.0f, 0.0f, 1.0f, 1.0f))
    , m_ClearBits(0)
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
    if (IsInitialized()) {
        return;
    }
    glGenFramebuffers(1, &m_Id);
    JAM_LOG("CFrameBufferOGLBase::Initialize() - id: %d\n", m_Id);
}

void CFrameBufferOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }
    glDeleteFramebuffers(1, &m_Id);
    JAM_LOG("CFrameBufferOGLBase::Shutdown() - id: %d\n", m_Id);
    m_Id = -1;
}

bool CFrameBufferOGLBase::IsInitialized()
{
    return (m_Id != -1);
}

void CFrameBufferOGLBase::Resize(uint64_t width, uint64_t height)
{
    m_Width = width;
    m_Height = height;

    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [width, height](IRenderTargetPtr renderTarget)
    {
        if (renderTarget)
        {
            renderTarget->Allocate(width, height);
        }
    });

    if (DepthAttachement()) {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(DepthAttachement())->Allocate(width, height);
    }

    if (StencilAttachement()) {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(StencilAttachement())->Allocate(width, height);
    }

    if (IsValid()) {
        glViewport(0, 0, width, height);
    }
    JAM_LOG("CFrameBufferOGLBase::Resize() - id: %d, size: (%llu, %llu)\n", m_Id, width, height);
}

uint64_t CFrameBufferOGLBase::MaxColorAttachements() const
{
    return m_NumColorAtachments;
}

void CFrameBufferOGLBase::AttachColor(IRenderTargetPtr colorTarget, uint64_t index)
{
    if (!colorTarget) {
        return;
    }

    if (index < MaxColorAttachements()) {
        colorTarget->Bind();

        colorTarget->Allocate(Width(), Height());
        IRenderTargetPtr renderBuffer = colorTarget->ColorTarget();
        IRenderTargetPtr renderTexture = colorTarget->TextureTarget();
        assert(renderBuffer || renderTexture);
        if (renderBuffer) {
            std::static_pointer_cast<CRenderTargetColorOGLBase>(renderBuffer)->BindToFrameBuffer(index);
        } else if (renderTexture) {
            std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsColorToFrameBuffer(index);
        }

        m_ColorBuffers[index] = colorTarget;
        m_ClearBits |= GL_COLOR_BUFFER_BIT;
        
        colorTarget->Unbind();
    }
}

void CFrameBufferOGLBase::AttachDepth(IRenderTargetPtr depthTarget)
{
    if (!depthTarget) {
        return;
    }

    depthTarget->Bind();
    
    depthTarget->Allocate(Width(), Height());
    IRenderTargetPtr renderBuffer = depthTarget->DepthTarget();
    IRenderTargetPtr renderTexture = depthTarget->TextureTarget();
    assert(renderBuffer || renderTexture);
    if (renderBuffer) {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(renderBuffer)->BindToFrameBuffer();
    } else if (renderTexture) {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsDepthToFrameBuffer();
    }

    m_DepthBuffer = depthTarget;
    m_ClearBits |= GL_DEPTH_BUFFER_BIT;
    if (depthTarget->StencilTarget()) {
        m_ClearBits |= GL_STENCIL_BUFFER_BIT;
    }
    
    depthTarget->Unbind();
}

void CFrameBufferOGLBase::AttachStencil(IRenderTargetPtr stencilTarget)
{
    if (!stencilTarget) {
        return;
    }

    stencilTarget->Bind();

    stencilTarget->Allocate(Width(), Height());
    IRenderTargetPtr renderBuffer = stencilTarget->StencilTarget();
    IRenderTargetPtr renderTexture = stencilTarget->TextureTarget();
    assert(renderBuffer || renderTexture);
    if (renderBuffer) {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(renderBuffer)->BindToFrameBuffer();
    } else if (renderTexture) {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderTexture)->BindAsStencilToFrameBuffer();
    }
    m_ClearBits |= GL_STENCIL_BUFFER_BIT;
    
    stencilTarget->Unbind();
}

void CFrameBufferOGLBase::DetachColor(uint64_t index)
{
    if (index < MaxColorAttachements()) {
        IRenderTargetPtr renderBuffer = m_ColorBuffers[index];
        renderBuffer->Bind();
        
        if (renderBuffer && renderBuffer->ColorTarget()) {
            std::static_pointer_cast<CRenderTargetColorOGLBase>(renderBuffer)->UnbindFromFrameBuffer(index);
        } else if (renderBuffer && renderBuffer->TextureTarget()) {
            std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsColorFromFrameBuffer(index);
        }
        m_ColorBuffers[index] = nullptr;
        
        bool haveAttachedColors = false;
        std::all_of(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](IRenderTargetPtr colorBuffer)
        {
            if (colorBuffer)
            {
                haveAttachedColors = true;
                return false;
            }
            return true;
        });
        if (!haveAttachedColors)
        {
            m_ClearBits &= ~GL_STENCIL_BUFFER_BIT;
        }
        
        renderBuffer->Bind();
    }
}

void CFrameBufferOGLBase::DetachDepth()
{
    IRenderTargetPtr renderBuffer = m_DepthBuffer;
    renderBuffer->Bind();
    
    if (renderBuffer && renderBuffer->DepthTarget()) {
        std::static_pointer_cast<CRenderTargetDepthOGLBase>(renderBuffer)->UnbindFromFrameBuffer();
    } else if (renderBuffer && renderBuffer->TextureTarget()) {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsDepthFromFrameBuffer();
    }
    m_ClearBits &= ~GL_DEPTH_BUFFER_BIT;
    if (m_DepthBuffer->StencilTarget()) {
        m_ClearBits &= GL_DEPTH_BUFFER_BIT;
    }
    m_DepthBuffer = nullptr;
    
    renderBuffer->Unbind();
}

void CFrameBufferOGLBase::DetachStencil()
{
    IRenderTargetPtr renderBuffer = m_StencilBuffer;
    renderBuffer->Bind();
    
    if (renderBuffer && renderBuffer->StencilTarget()) {
        std::static_pointer_cast<CRenderTargetStencilOGLBase>(renderBuffer)->UnbindFromFrameBuffer();
    } else if (renderBuffer && renderBuffer->TextureTarget()) {
        std::static_pointer_cast<CRenderTargetTextureOGLBase>(renderBuffer)->UnbindAsStencilFromFrameBuffer();
    }
    m_ClearBits &= GL_DEPTH_BUFFER_BIT;
    m_StencilBuffer = nullptr;
    
    renderBuffer->Unbind();
}

IRenderTargetPtr CFrameBufferOGLBase::ColorAttachement(uint64_t index) const
{
    if (index < MaxColorAttachements()) {
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
    if (DepthAttachement() && !m_StencilBuffer) {
        DepthAttachement()->StencilTarget();
    }

    return m_StencilBuffer;
}

bool CFrameBufferOGLBase::IsValid() const
{
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    return (result == GL_FRAMEBUFFER_COMPLETE);
}

void CFrameBufferOGLBase::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    JAM_LOG("CFrameBufferOGLBase::Bind() - id: %d\n", m_Id);

    if (IsValid()) {
        glViewport(0, 0, Width(), Height());
    }
}

void CFrameBufferOGLBase::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    JAM_LOG("CFrameBufferOGLBase::Unbind() - id: %d\n", m_Id);
}

void CFrameBufferOGLBase::Clear() const
{
    if (DepthAttachement()) {
        glEnable(GL_DEPTH_TEST);
    }
    
    glClearColor(m_ClearColor.R(), m_ClearColor.G(), m_ClearColor.B(), m_ClearColor.A());
    glClear(m_ClearBits);
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
    uint32_t rawdataSize = Width() * Height() * 4;
    IFrameBuffer::TRawData data(rawdataSize, 0);

#ifdef GL3_PROTOTYPES // TODO
    glReadPixels(0, 0, Width(), Height(), GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
#endif

    return data;
}

void CFrameBufferOGLBase::InitializeWithFBO(GLint fbo)
{
    m_Id = fbo;
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
