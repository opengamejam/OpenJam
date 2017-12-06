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

CRenderViewIOS::CRenderViewIOS(void* glkView, RenderApi renderApi)
    : IRenderView(((__bridge UIView*)glkView).frame.size.width,
          ((__bridge UIView*)glkView).frame.size.height,
          ((__bridge UIView*)glkView).contentScaleFactor)
    , m_GLKView((__bridge UIView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
{
}

CRenderViewIOS::~CRenderViewIOS()
{
}

void CRenderViewIOS::CreateView()
{
    if (m_RenderApi == Vulkan) {
        InitVulkan();
    } else {
        InitOGLES(m_RenderApi);
    }
}

void CRenderViewIOS::InitOGLES(RenderApi oglesVersion)
{
    switch (m_RenderApi) {
        case OGLES1_1: {
            EAGLContext* m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
        }
        break;
            
        case OGLES2_0: {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            [EAGLContext setCurrentContext:m_GLContext];
            
            m_Renderer.reset(new CRendererOGLES2_0(shared_from_this()));
        }
        break;
            
        case OGLES3_0: { // TODO: OGLES3
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            //m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
        }
        break;
            
        default:
            assert("Incorrect api version to initialize Open GL" && false);
        break;
    }
    
    m_RenderInstance.reset(new CRenderInstanceOGLBase(m_GLKView, m_GLContext));
    
    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);
    colorTarget->Bind();
    [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
    
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

void CRenderViewIOS::InitVulkan()
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
        std::tuple<VkResult, VkSurfaceKHR, std::vector<VkSurfaceFormatKHR> > surfaceData = CreateSurface(instance,
                                                                                                         physicalDevices[0],
                                                                                                         (__bridge void*)m_GLKView);
        VkResult result = std::get<0>(surfaceData);
        if (result != VK_SUCCESS) {
            JAM_LOG("Can't create Vulksn surface\n");
            return;
        }
        surface = std::get<1>(surfaceData);
        surfaceFormats = std::get<2>(surfaceData);
    }
    
    m_RenderInstance.reset(new CRenderInstanceVulkan(instance, physicalDevices[0], surface, surfaceFormats));
    m_Renderer.reset(new CRendererVulkan(shared_from_this()));
    
    
    // Swapchain
    VkSurfaceCapabilitiesKHR surfCapabilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[0], surface, &surfCapabilities);
    
    uint32_t presentModeCount;
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[0], surface, &presentModeCount, nullptr);
    
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[0], surface, &presentModeCount, &presentModes[0]);
    
    VkExtent2D swapchainExtent;
    // width and height are either both -1, or both not -1.
    if (surfCapabilities.currentExtent.width == (uint32_t)-1) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = Width(); // TODO
        swapchainExtent.height = Height();
    } else {
        swapchainExtent = surfCapabilities.currentExtent;
    }
    
    // If mailbox mode is available, use it, as is the lowest-latency non-
    // tearing mode.  If not, try IMMEDIATE which will usually be available,
    // and is fastest (though it tears).  If not, fall back to FIFO which is
    // always available.
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }
    
    // Determine the number of VkImage's to use in the swap chain (we desire to
    // own only 1 image at a time, besides the images being displayed and
    // queued for display):
    uint32_t desiredNumberOfSwapchainImages = surfCapabilities.minImageCount + 1;
    if ((surfCapabilities.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCapabilities.maxImageCount)) {
        // Application must settle for fewer images than desired:
        desiredNumberOfSwapchainImages = surfCapabilities.maxImageCount;
    }
    
    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        preTransform = surfCapabilities.currentTransform;
    }
    
    const VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .surface = surface,
        .minImageCount = desiredNumberOfSwapchainImages,
        .imageFormat = surfaceFormats[0].format,
        .imageColorSpace = surfaceFormats[0].colorSpace,
        .imageExtent = {
            .width = swapchainExtent.width,
            .height = swapchainExtent.height,
        },
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = preTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .imageArrayLayers = 1,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .presentMode = swapchainPresentMode,
        .oldSwapchain = VK_NULL_HANDLE,
        .clipped = true,
    };
    
    const VkDevice& logicalDevice = std::static_pointer_cast<CRendererVulkan>(m_Renderer)->LogicalDevice();
    
    result = vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &m_Swapchain);
    
    uint32_t swapchainImageCount;
    result = vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &swapchainImageCount, nullptr);
    
    std::vector<VkImage> swapchainImages(swapchainImageCount);
    assert(swapchainImageCount > 0);
    result = vkGetSwapchainImagesKHR(logicalDevice, m_Swapchain, &swapchainImageCount, &swapchainImages[0]);
    
    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };
    
    result = vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &m_Fence);
    assert(result == VK_SUCCESS);
    
    // Render targets
    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    std::static_pointer_cast<CRenderTargetColorVulkan>(colorTarget)->InitializeWithImages(swapchainImages, Width(), Height());
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);
    
    //CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    //depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);
    
    m_DefaultFrameBuffer = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    m_DefaultFrameBuffer->Initialize();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    //m_DefaultFrameBuffer->AttachDepth(depthTarget);
    
    std::static_pointer_cast<CRendererVulkan>(m_Renderer)->Initialize();
    assert(m_Renderer && m_DefaultFrameBuffer->IsValid());
}

void CRenderViewIOS::Begin() const
{
    if (m_RenderApi == Vulkan) {
        const VkDevice& logicalDevice = std::static_pointer_cast<CRendererVulkan>(m_Renderer)->LogicalDevice();
        
        VkResult result = vkAcquireNextImageKHR(logicalDevice, m_Swapchain,
                                                UINT64_MAX, VK_NULL_HANDLE,
                                                m_Fence, (uint32_t*)&m_SwapchainIndex);
        assert(result == VK_SUCCESS);
        
        result = vkWaitForFences(logicalDevice, 1, &m_Fence, VK_TRUE, UINT64_MAX);
        assert(result == VK_SUCCESS);
        
        result = vkResetFences(logicalDevice, 1, &m_Fence);
        assert(result == VK_SUCCESS);
    } else {
        [EAGLContext setCurrentContext:m_GLContext];
    }
}

void CRenderViewIOS::End() const
{
    if (m_RenderApi == Vulkan) {
        VkResult result;
        CRendererVulkanPtr vulkanRender = std::static_pointer_cast<CRendererVulkan>(m_Renderer);
        const VkQueue& queue = vulkanRender->Queue();
        const std::vector<VkCommandBuffer>& commandBuffers = vulkanRender->CommandBuffers();
        
        VkPresentInfoKHR presentInfoKHR = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &m_Swapchain,
            .pImageIndices = &m_SwapchainIndex,
            .pResults = &result
        };
        
        VkPipelineStageFlags waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = &waitMask,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffers[m_SwapchainIndex],
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr
        };
        
        result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        assert(result == VK_SUCCESS);
        
        result = vkQueueWaitIdle(queue);
        assert(result == VK_SUCCESS);
        
        result = vkQueuePresentKHR(queue, &presentInfoKHR);
        assert(result == VK_SUCCESS);
    } else {
        m_DefaultFrameBuffer->ColorAttachement(0)->Bind();
        [m_GLContext presentRenderbuffer:GL_RENDERBUFFER];
    }
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

// Vulkan

const VkSwapchainKHR& CRenderViewIOS::Swapchain() const
{
    return m_Swapchain;
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

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_IPHONE */
