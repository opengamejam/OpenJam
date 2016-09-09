//
//  CFrameBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0)

#ifndef CFRAMEBUFFEROGL1_3_H
#define CFRAMEBUFFEROGL1_3_H

#include "IFrameBuffer.h"

namespace jam
{

class CFrameBufferOGL1_3 : public IFrameBuffer
{
public:
    CFrameBufferOGL1_3(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGL1_3();
    
    /*
     * Initialize framebuffer object
     */
    virtual void Initialize() override;
    
    /*
     * Deinitialize framebuffer object
     */
    virtual void Shutdown() override;
    
    /*
     * Check if framebuffer is initialized
     */
    virtual bool IsInitialized() override;
    
    /*
     * Resize frame buffer
     */
    virtual void Resize(uint64_t width, uint64_t height) override;
    
    /*
     * Get maximum color attachements
     */
    virtual uint64_t MaxColorAttachements() const override;
    
    /*
     * Attach render buffer or render texture as color buffer.
     * Setup index to specify multiple render target
     */
    virtual void AttachColor(IRenderTargetPtr colorTarget, uint64_t index) override;
    
    /*
     * Attach render buffer or render texture as depth buffer
     */
    virtual void AttachDepth(IRenderTargetPtr colorTarget) override;
    
    /*
     * Attach render buffer or render texture as stencil buffer
     */
    virtual void AttachStencil(IRenderTargetPtr colorTarget) override;
    
    /*
     * Detach color buffer
     */
    virtual void DetachColor(uint64_t index) override;
    
    /*
     * Detach depth buffer
     */
    virtual void DetachDepth() override;
    
    /*
     * Detach stencil buffer
     */
    virtual void DetachStencil() override;
    
    /*
     * Returns attched color buffer for index
     */
    virtual IRenderTargetPtr ColorAttachement(uint64_t index) const override;
    
    /*
     * Returns attched depth buffer
     */
    virtual IRenderTargetPtr DepthAttachement() const override;
    
    /*
     * Returns attched stencil buffer
     */
    virtual IRenderTargetPtr StencilAttachement() const override;
    
    /*
     * Check if framebuffer is valid. For ex. OpenGL requires at least one
     * attached color buffers to be a valid framebuffer
     */
    virtual bool IsValid() const override;
    
    /*
     * Bind current framebuffer
     */
    virtual void Bind() const override;
    
    /*
     * Unbind framebuffer
     */
    virtual void Unbind() const override;
    
    /*
     * Clear all attached renderbuffers with color 'ClearColor'
     */
    virtual void Clear() const override;
    
    /*
     * Returns framebuffer width
     */
    virtual uint32_t Width() const override;
    
    /*
     * Returns framebuffer height
     */
    virtual uint32_t Height() const override;
    
    /*
     * Setup clear color which will fill color buffer after clear
     */
    virtual void ClearColor(const CColor4f& color) override;
    
    /*
     * Returns current clear color
     */
    virtual const CColor4f& ClearColor() const override;
    
    /*
     * Extract framebuffer raw image data (TODO: format)
     */
    virtual TRawData RawData() override;
    
private:
    uint32_t m_FrameBuffer;
    
    std::vector<IRenderTargetPtr> m_ColorBuffers;
    IRenderTargetPtr m_DepthBuffer;
    IRenderTargetPtr m_StencilBuffer;
    
    int m_NumColorAtachments;
    
    uint32_t m_Width;
    uint32_t m_Height;
    
    CColor4f m_ClearColor;
};

}; // namespace jam

#endif /* defined(CFRAMEBUFFEROGL1_3_H) */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) */