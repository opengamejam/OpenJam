 
    
//
//  CRenderTargetDepthVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
//#if defined(RENDER_VULKAN)

#ifndef CRENDERTARGETDEPTHVULKAN_H
#define CRENDERTARGETDEPTHVULKAN_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {
    
CLASS_PTR(CRendererVulkan)

class CRenderTargetDepthVulkan : public CRenderTargetDepth
{
    JAM_OBJECT
public:
    CRenderTargetDepthVulkan(CRendererVulkanPtr renderer);
    virtual ~CRenderTargetDepthVulkan();

    /*
     * Initialize render target object
     */
    virtual void Initialize(InternalFormats internalFormat) override;
    
    /*
     * Deinitialize render target object
     */
    virtual void Shutdown() override;
    
    /*
     * Check if render target is initialized
     */
    virtual bool IsInitialized() override;
    
    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) override;
    
    /*
     * Bind current render target
     */
    virtual void Bind() const override;
    
    /*
     * Unbind current render target
     */
    virtual void Unbind() const override;
    
    /*
     * Vulkan specific
     */
    const std::vector<VkImage>& Images() const;
    const std::vector<VkImageView>& ImageViews() const;
    static VkFormat ConvertInternalFormat(InternalFormats internalFormat);
    static InternalFormats ConvertInternalFormat(VkFormat vkFormat);
    
private:
    bool m_IsInitialized;
    CRendererVulkanWeak m_Renderer;
    VkFormat m_Format;
    std::vector<VkImage> m_Images;
    std::vector<VkImageView> m_ImageViews;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */

