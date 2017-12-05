//
//  CFrameBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CFRAMEBUFFERVULKAN_H
#define CFRAMEBUFFERVULKAN_H

#include "IFrameBuffer.h"

namespace jam {

CLASS_PTR(CFrameBufferVulkan)
CLASS_PTR(CRenderInstanceVulkan)
    
class CFrameBufferVulkan : public IFrameBuffer
{
    JAM_OBJECT
public:
    CFrameBufferVulkan(uint32_t width, uint32_t height,
                       const VkDevice& logicalDevice,
                       CRenderInstanceVulkanPtr renderInstance);
    virtual ~CFrameBufferVulkan();
    
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
    virtual uint64_t Width() const override;
    
    /*
     * Returns framebuffer height
     */
    virtual uint64_t Height() const override;
    
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
    
    /*
     * Vulkan specific
     */
    const std::vector<VkFramebuffer>& FrameBuffers() const;
    const VkRenderPass& RenderPass() const;
    
protected:
    void Rebuild();
    
private:
    bool m_IsInitialized;
    std::vector<IRenderTargetPtr> m_ColorBuffers;
    IRenderTargetPtr m_DepthBuffer;
    IRenderTargetPtr m_StencilBuffer;
    
    int m_NumColorAtachments;
    
    uint64_t m_Width;
    uint64_t m_Height;
    
    CColor4f m_ClearColor;
    
    VkDevice m_LogicalDevice;
    CRenderInstanceVulkanWeak m_RenderInstance;
    VkRenderPass m_RenderPass;
    std::vector<VkFramebuffer> m_Framebuffers;
};

}; // namespace jam

#endif /* CFRAMEBUFFERVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
