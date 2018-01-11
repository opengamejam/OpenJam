//
//  CRenderViewOSX.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_MAC)

#include "CRenderViewOSX.h"
#import <GLKit/GLKit.h>
#include "RenderGlobal.h"
#include "IFrameBuffer.h"
#include "IRenderTarget.h"

#include "CRendererOGL1_5.h"
#include "CFrameBufferOGL1_5.h"
#include "CRendererVulkan.h"

#include "CRenderInstanceVulkan.h"
#include "CRenderInstanceOGLBase.h"
#include "CRenderTargetColorVulkan.h"
#include <MoltenVK/vk_mvk_macos_surface.h>

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewOSX::CRenderViewOSX(NSView* view, float scaleFactor, RenderApi renderApi)
: IRenderView(view.frame.size.width,
              view.frame.size.height,
              scaleFactor)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
{
    if (m_RenderApi == Vulkan) {
        m_InitFunc = std::bind(&CRenderViewOSX::InitVulkan, this, view);
        m_BeginFunc = std::bind(&CRenderViewOSX::BeginVulkan, this);
        m_EndFunc = std::bind(&CRenderViewOSX::EndVulkan, this);
    } else {
        m_InitFunc = std::bind(&CRenderViewOSX::InitOGL, this, (NSOpenGLView*)view, m_RenderApi);
        m_BeginFunc = std::bind(&CRenderViewOSX::BeginOGL, this);
        m_EndFunc = std::bind(&CRenderViewOSX::EndOGL, this);
    }
}

CRenderViewOSX::~CRenderViewOSX()
{
}

void CRenderViewOSX::CreateView()
{
    m_InitFunc();
}

void CRenderViewOSX::Begin() const
{
    m_BeginFunc();
}

void CRenderViewOSX::End() const
{
    m_EndFunc();
}

void CRenderViewOSX::UpdateEvents() const
{
}

IRenderInstancePtr CRenderViewOSX::RenderInstance() const
{
    return m_RenderInstance;
}

IRendererPtr CRenderViewOSX::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewOSX::DefaultFrameBuffer() const
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
// Open GL
// *****************************************************************************

void CRenderViewOSX::InitOGL(NSOpenGLView *view, jam::CRenderViewOSX::RenderApi oglVersion)
{
    NSOpenGLContext *glContext = nil;
    NSOpenGLPixelFormat* pixelformat = nil;
    switch (oglVersion) {
    case OGLLegacy: {
        NSOpenGLPixelFormatAttribute attributes[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAColorSize, 32,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAStencilSize, 8,
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
            0
        };
        pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
        assert(pixelformat);
        glContext = [[NSOpenGLContext alloc] initWithFormat:pixelformat
                                                 shareContext:nil];
        [glContext makeCurrentContext];
        
        m_Renderer.reset(new CRendererOGL1_5(Ptr<CRenderViewOSX>()));
    } break;

    case OGL3_2: {
        /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            m_Renderer.reset(new CRendererOGLES2(shared_from_this()));*/
        //CGLEnable([m_GLContext CGLContextObj], kCGLCECrashOnRemovedFunctions);
    } break;

    case OGL4_1: {
        /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            m_Renderer.reset(new CRendererOGLES1(shared_from_this()));*/
    } break;

    default:
        break;
    };
    
    m_RenderInstance.reset(new CRenderInstanceOGLBase(view, glContext));
    
    [view setPixelFormat:pixelformat];
    [view setOpenGLContext:glContext];

    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);

    CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24);

    CRenderTargetStencilPtr stencilTarget = m_Renderer->CreateStencilRenderTarget();
    stencilTarget->Initialize(IRenderTarget::Stencil8);

    m_DefaultFrameBuffer = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    // Use 0 for the defaultFBO which is appropriate for
    // OSX (but not iOS since iOS apps must create their own FBO)
    m_DefaultFrameBuffer->Ptr<CFrameBufferOGLBase>()->InitializeWithFBO(0);
    m_DefaultFrameBuffer->Bind();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    m_DefaultFrameBuffer->AttachDepth(depthTarget);
    m_DefaultFrameBuffer->AttachStencil(stencilTarget);

    GLint swap = 1;
    [glContext setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

void CRenderViewOSX::BeginOGL()
{
    NSOpenGLContext* context = RenderInstance()->Ptr<CRenderInstanceOGLBase>()->Context();
    [context makeCurrentContext];
}

void CRenderViewOSX::EndOGL()
{
    NSOpenGLContext* context = RenderInstance()->Ptr<CRenderInstanceOGLBase>()->Context();
    [context flushBuffer];
}

// *****************************************************************************
// Vulkan
// *****************************************************************************

void CRenderViewOSX::InitVulkan(NSView* view)
{
    // Instance
    VkInstance instance = nullptr;
    {
        std::tuple<VkResult, VkInstance> instanceData = CreateInstance("CRenderViewMACOS", "Vulkan");
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

void CRenderViewOSX::BeginVulkan()
{
    Renderer()->Ptr<CRendererVulkan>()->Begin();
}

void CRenderViewOSX::EndVulkan()
{
    Renderer()->Ptr<CRendererVulkan>()->End();
}

std::tuple<VkResult, VkInstance> CRenderViewOSX::CreateInstance(const std::string& appName, const std::string& engineName)
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

std::tuple<VkResult, std::vector<VkPhysicalDevice> > CRenderViewOSX::GetPhysicalDevices(const VkInstance& instance)
{
    uint32_t physicalDeviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, &physicalDevices[0]);
    
    std::tuple<VkResult, std::vector<VkPhysicalDevice> > physycalDeviceData(result, physicalDevices);
    return physycalDeviceData;
}

std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> > CRenderViewOSX::CreateSurface(const VkInstance& instance,
                                                                                                   const VkPhysicalDevice& physicalDevice,
                                                                                                   NSView* view)
{
    VkResult result;
    VkSurfaceKHR surface;
#if defined(VK_USE_PLATFORM_MACOS_MVK)
    VkMacOSSurfaceCreateInfoMVK createInfo = {
        .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .flags = 0,
        .pView = (__bridge const void*)view
    };
    result = vkCreateMacOSSurfaceMVK(instance, &createInfo, nullptr, &surface);
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

#endif /* OS_MAC */
