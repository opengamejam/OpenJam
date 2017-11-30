//
//  CRenderInstanceVulkan.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 11/8/17.
//  Copyright © 2017 Yevgeniy Logachev. All rights reserved.
//

#ifndef CRENDERINSTANCEVULKAN_H
#define CRENDERINSTANCEVULKAN_H

#include "IRenderInstance.h"

namespace jam {

CLASS_PTR(CRenderInstanceVulkan)
    
class CRenderInstanceVulkan : public IRenderInstance
{
    JAM_OBJECT
public:
    CRenderInstanceVulkan(VkInstance instance,
                          VkPhysicalDevice physicalDevice,
                          VkSurfaceKHR surface,
                          const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
    virtual ~CRenderInstanceVulkan();
    
    /*
     * Initialize instance object
     */
    virtual void Initialize() override;
    
    /*
     * Deinitialize instance object
     */
    virtual void Shutdown() override;
    
    /*
     * Check if instance is initialized
     */
    virtual bool IsInitialized() const override;
    
    /*
     * Vulkan specific
     * Get Vulkan instance
     */
    const VkInstance& Instance() const;
    
    /*
     * Get Vulkan physical device
     */
    const VkPhysicalDevice& PhysicalDevice() const;
    
    /*
     * Get Vulkan surface
     */
    const VkSurfaceKHR& Surface() const;
    
    /*
     * Get Vulkan surface formats
     */
    const std::vector<VkSurfaceFormatKHR>& SurfaceFormats() const;
    
private:
    bool m_IsInitialized;
    VkInstance m_Instance;
    VkPhysicalDevice m_PhysicalDevice;
    VkSurfaceKHR m_Surface;
    std::vector<VkSurfaceFormatKHR> m_SurfaceFormats;
};
    
}; // namespace jam

#endif /* CRENDERINSTANCEVULKAN_H */
