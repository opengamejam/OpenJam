//
//  CRenderTargetColorVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
//#if defined(RENDER_VULKAN)

#ifndef CRENDERTARGETCOLORVULKAN_H
#define CRENDERTARGETCOLORVULKAN_H

#include "CRenderTargetColorOGLBase.h"

namespace jam {
    
CLASS_PTR(CRenderTargetColorVulkan)
CLASS_PTR(CRendererVulkan)

class CRenderTargetColorVulkan : public CRenderTargetColor
{
    JAM_OBJECT
public:
    CRenderTargetColorVulkan(CRendererVulkanPtr renderer);
    virtual ~CRenderTargetColorVulkan();
    
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
     * Get internal format
     */
    virtual InternalFormats InternalFormat() const override;

    /*
     * Vulkan specific
     */
    virtual void InitializeWithImages(const std::vector<VkImage>& images,
                                      uint64_t width, uint64_t height);
    const std::vector<VkImage>& Images() const;
    const std::vector<VkImageView>& ImageViews() const;
    static VkFormat ConvertInternalFormat(InternalFormats internalFormat);
    static InternalFormats ConvertInternalFormat(VkFormat vkFormat);
    VkFormat Format() const;
    
private:
    bool m_IsInitialized;
    CRendererVulkanWeak m_Renderer;
    VkFormat m_Format;
    std::vector<VkImage> m_Images;
    std::vector<VkImageView> m_ImageViews;
};

}; // namespace jam

#endif /* CRENDERTARGETCOLORVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
