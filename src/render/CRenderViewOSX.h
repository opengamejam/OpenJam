//
//  CRenderViewOSX.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_MAC)

#ifndef CRENDERVIEWOSX_H
#define	CRENDERVIEWOSX_H

#include "IRenderView.h"
#if defined(__OBJC__)
@class MTKView;
#endif

namespace jam
{
class CRenderViewOSX : public IRenderView
{
    JAM_OBJECT
public:
    enum RenderApi
    {
        OGLLegacy,
        OGL3_2,
        OGL4_1,
        Vulkan
    };
    
public:
	CRenderViewOSX(NSView* view, float scaleFactor, RenderApi renderApi);
	virtual ~CRenderViewOSX();

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
                                                                                       NSView* view);
private:
    void InitOGL(NSOpenGLView* view, RenderApi oglVersion);
    void InitVulkan(MTKView* view);
    
    void BeginOGL();
    void BeginVulkan();
    
    void EndOGL();
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

#endif	/* CRENDERVIEWOSX_H */

#endif /* OS_MAC */

