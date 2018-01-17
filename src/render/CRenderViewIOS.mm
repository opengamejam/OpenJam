//
//  CRenderViewIOS.mm
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_IPHONE)

#include "CRenderViewIOS.h"
#import <OpenGLES/EAGL.h>
#import "GLKit/GLKit.h"
#include "RenderGlobal.h"
#include "IFrameBuffer.h"
#include "IRenderTarget.h"

#include "CRendererOGLES1_1.h"
#include "CRendererOGLES2_0.h"
#include "CRendererVulkan.h"

#include "CRenderInstanceVulkan.h"
#include "CRenderInstanceOGLBase.h"
#include "CRenderTargetColorVulkan.h"
#include "CRenderTargetDepthVulkan.h"
#include "CFrameBufferVulkan.h"
#include <MoltenVK/mvk_vulkan.h>
#include <MoltenVK/mvk_datatypes.h>

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewIOS::CRenderViewIOS(UIView* view, RenderApi renderApi)
    : IRenderView(view.frame.size.width,
                  view.frame.size.height,
                  view.contentScaleFactor)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
{
    if (m_RenderApi == Vulkan) {
        assert([view isKindOfClass:[MTKView class]]);
        m_InitFunc = std::bind(&CRenderViewIOS::InitVulkan, this, (MTKView*)view);
        m_BeginFunc = std::bind(&CRenderViewIOS::BeginVulkan, this);
        m_EndFunc = std::bind(&CRenderViewIOS::EndVulkan, this);
    } else {
        assert([view isKindOfClass:[GLKView class]]);
        m_InitFunc = std::bind(&CRenderViewIOS::InitOGLES, this, (GLKView*)view, m_RenderApi);
        m_BeginFunc = std::bind(&CRenderViewIOS::BeginOGLES, this);
        m_EndFunc = std::bind(&CRenderViewIOS::EndOGLES, this);
    }
}

CRenderViewIOS::~CRenderViewIOS()
{
}

void CRenderViewIOS::CreateView()
{
    m_InitFunc();
}

void CRenderViewIOS::Begin() const
{
    m_BeginFunc();
}

void CRenderViewIOS::End() const
{
    m_EndFunc();
}

void CRenderViewIOS::UpdateEvents() const
{
}

IRenderInstancePtr CRenderViewIOS::RenderInstance() const
{
    return m_RenderInstance;
}

IRendererPtr CRenderViewIOS::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewIOS::DefaultFrameBuffer() const
{
    return m_DefaultFrameBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

// *****************************************************************************
// OGLES
// *****************************************************************************

void CRenderViewIOS::InitOGLES(GLKView* view, RenderApi oglesVersion)
{
    EAGLContext* glContext = nil;
    switch (oglesVersion) {
        case OGLES1_1: {
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            m_Renderer.reset(new CRendererOGLES1_1(Ptr<IRenderView>()));
        }
        break;
            
        case OGLES2_0: {
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            m_Renderer.reset(new CRendererOGLES2_0(Ptr<IRenderView>()));
        }
        break;
            
        case OGLES3_0: { // TODO: OGLES3
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            //m_Renderer.reset(new CRendererOGLES3_0(Ptr<IRenderView>()));
        }
        break;
            
        default:
            assert("Incorrect api version to initialize Open GL" && false);
        break;
    }
    
    [EAGLContext setCurrentContext:glContext];
    m_RenderInstance.reset(new CRenderInstanceOGLBase(view, glContext));
    
    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(ConvertColorFormat(view.drawableColorFormat));
    colorTarget->Bind();
    [glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(view.layer)];
    
    CRenderTargetDepthPtr depthTarget = nullptr;
    if (view.drawableDepthFormat != GLKViewDrawableDepthFormatNone) {
        depthTarget = m_Renderer->CreateDepthRenderTarget();
        depthTarget->Initialize(ConvertDepthStencilFormat(view.drawableDepthFormat,
                                                          view.drawableStencilFormat));
    }
    
    m_DefaultFrameBuffer = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    m_DefaultFrameBuffer->Initialize();
    m_DefaultFrameBuffer->Bind();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    m_DefaultFrameBuffer->AttachDepth(depthTarget);
    
    m_DefaultFrameBuffer->Bind();
    assert(m_Renderer && m_DefaultFrameBuffer->IsValid());
}

void CRenderViewIOS::BeginOGLES()
{
    EAGLContext* context = RenderInstance()->Ptr<CRenderInstanceOGLBase>()->Context();
    [EAGLContext setCurrentContext:context];
}

void CRenderViewIOS::EndOGLES()
{
    m_DefaultFrameBuffer->ColorAttachement(0)->Bind();
    EAGLContext* context = RenderInstance()->Ptr<CRenderInstanceOGLBase>()->Context();
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

IRenderTarget::InternalFormats CRenderViewIOS::ConvertColorFormat(GLKViewDrawableColorFormat colorFormat)
{
    IRenderTarget::InternalFormats internalFormat = IRenderTarget::ColorRGBA8888;
    switch (colorFormat) {
        case GLKViewDrawableColorFormatRGBA8888:
            internalFormat = IRenderTarget::ColorRGBA8888;
            break;
        case GLKViewDrawableColorFormatRGB565:
            internalFormat = IRenderTarget::ColorRGB565;
            break;
        case GLKViewDrawableColorFormatSRGBA8888:
            internalFormat = IRenderTarget::ColorRGBA8888;
            break;
            
        default:
            JAM_LOG("Unknown OGLES color format");
            break;
    };
    return internalFormat;
}

IRenderTarget::InternalFormats CRenderViewIOS::ConvertDepthStencilFormat(GLKViewDrawableDepthFormat depthFormat,
                                                                         GLKViewDrawableStencilFormat stencilFormat)
{
    IRenderTarget::InternalFormats internalFormat = IRenderTarget::Depth24_Stencil8;
    switch (depthFormat) {
        case GLKViewDrawableDepthFormat16:
            internalFormat = IRenderTarget::Depth16;
            break;
        case GLKViewDrawableDepthFormat24:
            internalFormat = IRenderTarget::Depth24;
            if (stencilFormat == GLKViewDrawableStencilFormat8) {
                internalFormat = IRenderTarget::Depth24_Stencil8;
            }
            break;
            
        default:
            JAM_LOG("Unknown OGLES depth/stencil format");
            break;
    };
    return internalFormat;
}

// *****************************************************************************
// Vulkan
// *****************************************************************************

void CRenderViewIOS::InitVulkan(MTKView* view)
{
    // Instance
    VkInstance instance = nullptr;
    {
        std::tuple<VkResult, VkInstance> instanceData = CreateInstance("CRenderViewIOS", "Vulkan");
        VkResult result = std::get<0>(instanceData);
        if (result != VK_SUCCESS) {
            JAM_LOG("Can't create Vulkan instance\n");
            return;
        }
        instance = std::get<1>(instanceData);
    }
    assert(instance);
    
    // GPU
    std::vector<VkPhysicalDevice> physicalDevices;
    {
        std::tuple<VkResult, std::vector<VkPhysicalDevice> > physicalDeviceData = GetPhysicalDevices(instance);
        VkResult result = std::get<0>(physicalDeviceData);
        if (result != VK_SUCCESS) {
            JAM_LOG("Can't retrieve Vulkan compatible GPU list\n");
            return;
        }
        physicalDevices = std::get<1>(physicalDeviceData);
    }
    assert(physicalDevices.size() > 0);
    
    // Surface
    VkSurfaceKHR surface;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    {
        typedef std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> > TSurfaceData;
        TSurfaceData surfaceData = CreateSurface(instance, physicalDevices[0], view);
        VkResult result = std::get<0>(surfaceData);
        if (result != VK_SUCCESS) {
            JAM_LOG("Can't create Vulksn surface\n");
            return;
        }
        surface = std::get<1>(surfaceData);
        surfaceFormats = std::get<2>(surfaceData);
    }
    
    m_RenderInstance.reset(new CRenderInstanceVulkan(instance, physicalDevices[0], surface, surfaceFormats));
    m_Renderer.reset(new CRendererVulkan(Ptr<IRenderView>()));
    
    // Render targets
    CRenderTargetColorVulkanPtr colorTarget = Renderer()->CreateColorRenderTarget()->Ptr<CRenderTargetColorVulkan>();
    colorTarget->Initialize(CRenderTargetColorVulkan::ConvertInternalFormat(surfaceFormats[0].format));
    colorTarget->InitializeWithImages(Renderer()->Ptr<CRendererVulkan>()->SwapchainImages(),
                                      RealWidth(), RealHeight());
    
    CRenderTargetDepthPtr depthTarget = nullptr;
    if (view.depthStencilPixelFormat != MTLPixelFormatInvalid) {
        depthTarget = Renderer()->CreateDepthRenderTarget();
        VkFormat vkFormat = mvkVkFormatFromMTLPixelFormat(view.depthStencilPixelFormat);
        IRenderTarget::InternalFormats internalFormat = CRenderTargetDepthVulkan::ConvertInternalFormat(vkFormat);
        depthTarget->Initialize(internalFormat);
        depthTarget->Allocate(RealWidth(), RealHeight());
    }
    
    m_DefaultFrameBuffer = Renderer()->CreateFrameBuffer(Width(), Height());
    m_DefaultFrameBuffer->Initialize();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    m_DefaultFrameBuffer->AttachDepth(depthTarget);
    assert(m_DefaultFrameBuffer->IsValid());
    
    Renderer()->Ptr<CRendererVulkan>()->CreateCommandBuffers();
}

void CRenderViewIOS::BeginVulkan()
{
    Renderer()->Ptr<CRendererVulkan>()->Begin();
}

void CRenderViewIOS::EndVulkan()
{
    Renderer()->Ptr<CRendererVulkan>()->End();
}

std::tuple<VkResult, VkInstance> CRenderViewIOS::CreateInstance(const std::string& appName, const std::string& engineName)
{
    const VkApplicationInfo app = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = appName.c_str(),
        .applicationVersion = 0,
        .pEngineName = engineName.c_str(),
        .engineVersion = 0,
        .apiVersion = VK_API_VERSION_1_0,
    };
    VkInstanceCreateInfo inst_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &app,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = 0,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = 0,
    };
    
    VkInstance instance;
    VkResult result = vkCreateInstance(&inst_info, nullptr, &instance);
    std::tuple<VkResult, VkInstance> instanceData(result, instance);
    return instanceData;
}

std::tuple<VkResult, std::vector<VkPhysicalDevice> > CRenderViewIOS::GetPhysicalDevices(const VkInstance& instance)
{
    uint32_t physicalDeviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, &physicalDevices[0]);
    
    std::tuple<VkResult, std::vector<VkPhysicalDevice> > physycalDeviceData(result, physicalDevices);
    return physycalDeviceData;
}

std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> > CRenderViewIOS::CreateSurface(const VkInstance& instance,
                                                                                                   const VkPhysicalDevice& physicalDevice,
                                                                                                   MTKView* view)
{
    VkResult result;
    VkSurfaceKHR surface;
#if defined(VK_USE_PLATFORM_IOS_MVK)
    VkIOSSurfaceCreateInfoMVK createInfo = {
        .sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .flags = 0,
        .pView = (__bridge const void*)view
    };
    result = vkCreateIOSSurfaceMVK(instance, &createInfo, nullptr, &surface);
    assert(result == VK_SUCCESS);
#endif
    
    uint32_t formatCount;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    
    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, &surfaceFormats[0]);
    
    if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
        surfaceFormats[0].format = VK_FORMAT_B8G8R8A8_UNORM;
    }
    
    return std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> >(result, surface, surfaceFormats);
}

#endif /* OS_IPHONE */
