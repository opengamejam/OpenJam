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
#include <MoltenVK/vk_mvk_ios_surface.h>

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewIOS::CRenderViewIOS(void* view, RenderApi renderApi)
    : IRenderView(((__bridge UIView*)view).frame.size.width,
                  ((__bridge UIView*)view).frame.size.height,
                  ((__bridge UIView*)view).contentScaleFactor)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
{
    if (m_RenderApi == Vulkan) {
        m_InitFunc = std::bind(&CRenderViewIOS::InitVulkan, this, (__bridge UIView*)view);
        m_BeginFunc = std::bind(&CRenderViewIOS::BeginVulkan, this);
        m_EndFunc = std::bind(&CRenderViewIOS::EndVulkan, this);
    } else {
        m_InitFunc = std::bind(&CRenderViewIOS::InitOGLES, this, (__bridge UIView*)view, m_RenderApi);
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

void CRenderViewIOS::InitOGLES(UIView* view, RenderApi oglesVersion)
{
    EAGLContext* glContext = nil;
    switch (m_RenderApi) {
        case OGLES1_1: {
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:glContext];
            
            m_Renderer.reset(new CRendererOGLES1_1(Ptr<IRenderView>()));
        }
        break;
            
        case OGLES2_0: {
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            [EAGLContext setCurrentContext:glContext];
            
            m_Renderer.reset(new CRendererOGLES2_0(Ptr<IRenderView>()));
        }
        break;
            
        case OGLES3_0: { // TODO: OGLES3
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:glContext];
            
            //m_Renderer.reset(new CRendererOGLES3_0(Ptr<IRenderView>()));
        }
        break;
            
        default:
            assert("Incorrect api version to initialize Open GL" && false);
        break;
    }
    
    m_RenderInstance.reset(new CRenderInstanceOGLBase(view, glContext));
    
    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);
    colorTarget->Bind();
    [glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(view.layer)];
    
    CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);
    
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

// *****************************************************************************
// Vulkan
// *****************************************************************************

void CRenderViewIOS::InitVulkan(UIView* view)
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
        TSurfaceData surfaceData = CreateSurface(instance, physicalDevices[0], (__bridge void*)view);
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
    colorTarget->InitializeWithImages(Renderer()->Ptr<CRendererVulkan>()->SwapchainImages(),
                                      surfaceFormats[0].format,
                                      Width(), Height());
    
    //CRenderTargetDepthPtr depthTarget = Renderer()->CreateDepthRenderTarget();
    //depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);
    
    m_DefaultFrameBuffer = Renderer()->CreateFrameBuffer(Width(), Height());
    m_DefaultFrameBuffer->Initialize();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    //m_DefaultFrameBuffer->AttachDepth(depthTarget);
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
                                                                                                   void* view)
{
    VkResult result;
    VkSurfaceKHR surface;
#if defined(VK_USE_PLATFORM_IOS_MVK)
    VkIOSSurfaceCreateInfoMVK createInfo = {
        .sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .flags = 0,
        .pView = view
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
