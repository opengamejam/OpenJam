//
//  CRenderViewIOS.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_IPHONE)

#ifndef CRENDERVIEWIOS_H
#define CRENDERVIEWIOS_H

#include "IRenderView.h"

namespace jam {
class CRenderViewIOS : public IRenderView
{
    JAM_OBJECT
public:
    enum RenderApi {
        OGLES1_1,
        OGLES2_0,
        OGLES3_0,
        Vulkan
    };

public:
    CRenderViewIOS(void* view, RenderApi renderApi);
    virtual ~CRenderViewIOS();

    virtual void CreateView() override;
    virtual void Begin() const override;
    virtual void End() const override;
    virtual void UpdateEvents() const override;

    virtual IRenderInstancePtr RenderInstance() const override;
    virtual IRendererPtr Renderer() const override;
    virtual IFrameBufferPtr DefaultFrameBuffer() const override;

    /*
     * Vulkan specific
     */
    std::tuple<VkResult, VkInstance> CreateInstance(const std::string& appName, const std::string& engineName);
    std::tuple<VkResult, std::vector<VkPhysicalDevice> > GetPhysicalDevices(const VkInstance& instance);
    std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> > CreateSurface(const VkInstance& instance,
                                                                                       const VkPhysicalDevice& physicalDevice,
                                                                                       void* view);
private:
    void InitOGLES(UIView* view, RenderApi oglesVersion);
    void InitVulkan(UIView* view);
    
    void BeginOGLES();
    void BeginVulkan();
    
    void EndOGLES();
    void EndVulkan();
    
private:
    RenderApi m_RenderApi;
    IRenderInstancePtr m_RenderInstance;
    IRendererPtr m_Renderer;
    IFrameBufferPtr m_DefaultFrameBuffer;
    
    std::function<void()> m_InitFunc;
    std::function<void()> m_BeginFunc;
    std::function<void()> m_EndFunc;
};

}; // namespace jam

#endif /* CRENDERVIEWIOS_H */

#endif /* OS_IPHONE */

