//
//  CTextureVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CTEXTUREVULKAN_H
#define CTEXTUREVULKAN_H

#include "ITexture.h"

namespace jam {
    
CLASS_PTR(CRendererVulkan)

class CTextureVulkan : public ITexture
{
    JAM_OBJECT
public:
    CTextureVulkan(CRendererVulkanPtr renderer);
    virtual ~CTextureVulkan();

    /*
     * Allocate render buffer with 'width' and 'height'
     */
    virtual void Allocate(uint64_t width, uint64_t height) override;
    
    /*
     * Assign image to texture
     */
    virtual bool AssignImage(IImagePtr image) override;
    
    virtual void Filter(TextureFilters filter) override;
    virtual TextureFilters Filter() const override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual const std::string& Hash() override;
    
private:
    void HashMe();
    
private:
    TextureFilters m_Filter;
    std::string m_Hash;
    bool m_IsDirty;
    
    CRendererVulkanWeak m_Renderer;
    VkImage m_Image;
    VkDeviceMemory m_DeviceMemory;
    VkImageView m_ImageView;
    VkSampler m_Sampler;
};

}; // namespace jam

#endif /* CTEXTUREVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */


