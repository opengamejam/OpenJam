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
#if defined(__OBJC__)
#import <GLKit/GLKit.h>
@class MTKView;
#endif

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
    CRenderViewIOS(UIView* view, RenderApi renderApi);
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
                                                                                       MTKView* view);
private:
    void InitOGLES(GLKView* view, RenderApi oglesVersion);
    void BeginOGLES();
    void EndOGLES();
    IRenderTarget::InternalFormats ConvertColorFormat(GLKViewDrawableColorFormat colorFormat);
    IRenderTarget::InternalFormats ConvertDepthStencilFormat(GLKViewDrawableDepthFormat depthFormat,
                                                             GLKViewDrawableStencilFormat stencilFormat);
    
    void InitVulkan(MTKView* view);
    void BeginVulkan();
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

