//
//  CRendererVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CRendererVulkan.h"

#include "IRenderView.h"
#include "CVertexBufferVulkan.h"
#include "CIndexBufferVulkan.h"
#include "CMaterialVulkan.h"
#include "CTextureVulkan.h"
#include "CMeshVulkan.h"
#include "CShaderVulkan.h"
#include "CShaderProgramVulkan.h"
#include "CFrameBufferVulkan.h"
#include "CRenderTargetColorVulkan.h"
#include "CRenderTargetDepthVulkan.h"
#include "CRenderTargetStencilVulkan.h"
#include "CRenderTargetTextureVulkan.h"
#include "CRenderInstanceVulkan.h"
#include "CRenderTargetColorVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererVulkan::CRendererVulkan(IRenderViewPtr renderView)
    : m_RenderView(renderView)
    , m_SwapchainIndex(0)
{
    CRenderInstanceVulkanPtr instance = RenderView()->RenderInstance()->Ptr<CRenderInstanceVulkan>();
    // GPU Props
    VkPhysicalDeviceProperties pProperties;
    {
        
        vkGetPhysicalDeviceProperties(instance->PhysicalDevice(), &pProperties);
    }
    
    // Logical device
    {
        std::tuple<VkResult, VkDevice> logicalDeviceData = CreateLogicalDevice(instance->PhysicalDevice());
        VkResult result = std::get<0>(logicalDeviceData);
        if (result != VK_SUCCESS) {
            JAM_LOG("Can't create Vulksn logical device\n");
            return;
        }
        m_LogicalDevice = std::get<1>(logicalDeviceData);
    }
    
    // GPU Queue Props
    {
        m_QueueProperties = GetPhysicalDeviceQueueProps(instance->Instance(), instance->PhysicalDevice());
    }
    
    // Command pool
    uint32_t graphicQueuePropsIndex = 0;
    {
        std::vector<VkBool32> supportsPresent(m_QueueProperties.size());
        for (int i = 0; i < m_QueueProperties.size(); i++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(instance->PhysicalDevice(), i, instance->Surface(), &supportsPresent[i]);
        }
        
        std::tuple<bool, VkQueueFamilyProperties, uint32_t> graphicQueuePropsData = FindQueueProperties(VK_QUEUE_GRAPHICS_BIT,
                                                                                                        supportsPresent);
        bool found = std::get<0>(graphicQueuePropsData);
        if (!found) {
            JAM_LOG("Can't find Vulksn graphics queue\n");
            return;
        }
        graphicQueuePropsIndex = std::get<2>(graphicQueuePropsData);
        vkGetDeviceQueue(m_LogicalDevice, graphicQueuePropsIndex, 0, &m_Queue);
    }
    
    const VkCommandPoolCreateInfo cmdPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .queueFamilyIndex = graphicQueuePropsIndex,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    };
    
    VkResult result = vkCreateCommandPool(m_LogicalDevice, &cmdPoolInfo, nullptr, &m_CommandPool);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create command pool");
        return;
    }
    result = vkResetCommandPool(m_LogicalDevice, m_CommandPool,
                                VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
    if (result != VK_SUCCESS) {
        JAM_LOG("Invalid command pool");
        return;
    }
    
    // Swapchain
    VkSurfaceCapabilitiesKHR surfCapabilities;
    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(instance->PhysicalDevice(),
                                                       instance->Surface(),
                                                       &surfCapabilities);
    
    uint32_t presentModeCount;
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(instance->PhysicalDevice(),
                                                       instance->Surface(),
                                                       &presentModeCount, nullptr);
    
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(instance->PhysicalDevice(),
                                                       instance->Surface(),
                                                       &presentModeCount,
                                                       &presentModes[0]);
    
    VkExtent2D swapchainExtent;
    // width and height are either both -1, or both not -1.
    if (surfCapabilities.currentExtent.width == (uint32_t)-1) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = RenderView()->Width(); // TODO
        swapchainExtent.height = RenderView()->Height();
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
        .surface = instance->Surface(),
        .minImageCount = desiredNumberOfSwapchainImages,
        .imageFormat = instance->SurfaceFormats()[0].format,
        .imageColorSpace = instance->SurfaceFormats()[0].colorSpace,
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
    
    result = vkCreateSwapchainKHR(m_LogicalDevice, &swapchainInfo, nullptr, &m_Swapchain);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create swapchain object");
        return;
    }
    
    uint32_t swapchainImageCount;
    result = vkGetSwapchainImagesKHR(m_LogicalDevice, m_Swapchain, &swapchainImageCount, nullptr);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create swapchain images");
        return;
    }
    
    m_SwapchainImages.resize(swapchainImageCount);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create swapchain images");
        return;
    }
    result = vkGetSwapchainImagesKHR(m_LogicalDevice, m_Swapchain, &swapchainImageCount, &m_SwapchainImages[0]);
    
    // Swapchain fence
    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };
    
    result = vkCreateFence(m_LogicalDevice, &fenceCreateInfo, nullptr, &m_SwapchainFence);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create swapchain fence");
        return;
    }
}

CRendererVulkan::~CRendererVulkan()
{
}

void CRendererVulkan::CreateCommandBuffers()
{
    IFrameBufferPtr defaultFrameBuffer = RenderView()->DefaultFrameBuffer();
    CFrameBufferVulkanPtr frameBuffer = defaultFrameBuffer->Ptr<CFrameBufferVulkan>();
    uint32_t count = (uint32_t)frameBuffer->FrameBuffers().size();
    const VkCommandBufferAllocateInfo cmd = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = m_CommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = count,
    };
    
    m_CommandBuffers.resize(count);
    VkResult result = vkAllocateCommandBuffers(m_LogicalDevice, &cmd, m_CommandBuffers.data());
    assert(result == VK_SUCCESS);
    
    const VkRenderPass& renderPass = frameBuffer->RenderPass();
    for (uint32_t i = 0; i < frameBuffer->FrameBuffers().size(); ++i) {
        const VkFramebuffer& fb = frameBuffer->FrameBuffers()[i];
        const VkCommandBuffer& commandBuffer = m_CommandBuffers[i];
        
        VkCommandBufferInheritanceInfo commandBufferInheritanceInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
            .pNext = nullptr,
            .renderPass = renderPass,
            .subpass = 0,
            .framebuffer = fb,
            .occlusionQueryEnable = VK_FALSE,
            .queryFlags = 0,
            .pipelineStatistics = 0
        };
        
        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = &commandBufferInheritanceInfo
        };
        
        vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
        {
            
            VkClearValue clearValue = {
                .color.float32[0] = 1.0f,
                .color.float32[1] = 0.0f,
                .color.float32[2] = 0.0f,
                .color.float32[3] = 1.0f,
                .depthStencil.depth = 1.0f,
                .depthStencil.stencil = 0,
            };
            
            VkRenderPassBeginInfo renderPassBeginInfo = {
                .sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext = nullptr,
                .renderPass = renderPass,
                .framebuffer = fb,
                .renderArea = VkRect2D{
                    VkOffset2D{ 0, 0 },
                    VkExtent2D{
                        RenderView()->Width(),
                        RenderView()->Height()
                    }
                },
                .clearValueCount = 1,
                .pClearValues = &clearValue
            };
            
            vkCmdBeginRenderPass(commandBuffer,
                                 &renderPassBeginInfo,
                                 VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
            //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pip);
            vkCmdEndRenderPass(commandBuffer);
            
            if (frameBuffer->ColorAttachement(0)) {
                CRenderTargetColorVulkanPtr colorTarget = frameBuffer->ColorAttachement(0)->Ptr<CRenderTargetColorVulkan>();
                VkImageMemoryBarrier imageMemoryBarrier = {
                    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .pNext = nullptr,
                    .srcAccessMask = 0,
                    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_MEMORY_READ_BIT,
                    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    .image = colorTarget->Images()[i],
                    .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
                };
            
                vkCmdPipelineBarrier(commandBuffer,
                                     VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                     VK_PIPELINE_STAGE_TRANSFER_BIT,
                                     0,
                                     0, nullptr,
                                     0, nullptr,
                                     1, &imageMemoryBarrier);
            }
            
            if (frameBuffer->DepthAttachement()) {
                CRenderTargetColorVulkanPtr depthTarget = frameBuffer->DepthAttachement()->Ptr<CRenderTargetColorVulkan>();
                VkImageMemoryBarrier imageMemoryBarrier = {
                    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .pNext = nullptr,
                    .srcAccessMask = 0,
                    .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                    .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                    .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    .image = depthTarget->Images()[0],
                    .subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 }
                };
                
                vkCmdPipelineBarrier(commandBuffer,
                                     VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                     VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                                     0,
                                     0, nullptr,
                                     0, nullptr,
                                     1, &imageMemoryBarrier);
            }
        }
        vkEndCommandBuffer(commandBuffer);
    }
}

void CRendererVulkan::Begin()
{
    VkResult result = vkAcquireNextImageKHR(m_LogicalDevice, m_Swapchain,
                                            UINT64_MAX, VK_NULL_HANDLE,
                                            m_SwapchainFence, &m_SwapchainIndex);
    assert(result == VK_SUCCESS);
    
    result = vkWaitForFences(m_LogicalDevice, 1, &m_SwapchainFence, VK_TRUE, UINT64_MAX);
    assert(result == VK_SUCCESS);
    
    result = vkResetFences(m_LogicalDevice, 1, &m_SwapchainFence);
    assert(result == VK_SUCCESS);
}

void CRendererVulkan::End()
{
    const VkQueue& queue = Queue();
    const VkCommandBuffer& commandBuffer = CommandBuffer(m_SwapchainIndex);
    
    VkResult result = VK_SUCCESS;
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
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr
    };
    
    result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    assert(result == VK_SUCCESS);
    
    result = vkQueueWaitIdle(queue);
    assert(result == VK_SUCCESS);
    
    result = vkQueuePresentKHR(queue, &presentInfoKHR);
    assert(result == VK_SUCCESS);
}

IRenderViewPtr CRendererVulkan::RenderView() const
{
    return m_RenderView;
}

IFrameBufferPtr CRendererVulkan::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferVulkan(width, height, Ptr<CRendererVulkan>()));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererVulkan::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorVulkan(Ptr<CRendererVulkan>()));
    return colorTarget;
}

CRenderTargetDepthPtr CRendererVulkan::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthVulkan(Ptr<CRendererVulkan>()));
    return depthTarget;
}

CRenderTargetStencilPtr CRendererVulkan::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget;//(new CRenderTargetStencilVulkan());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererVulkan::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget;//(new CRenderTargetTextureVulkan());
    return textureTarget;
}

IVertexBufferPtr CRendererVulkan::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferVulkan(Ptr<CRendererVulkan>()));
    return vertexBuffer;
}

IIndexBufferPtr CRendererVulkan::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferVulkan(Ptr<CRendererVulkan>()));
    return indexBuffer;
}

ITexturePtr CRendererVulkan::CreateTexture()
{
    ITexturePtr texture(new CTextureVulkan(Ptr<CRendererVulkan>()));
    return texture;
}

IMaterialPtr CRendererVulkan::CreateMaterial()
{
    IMaterialPtr material;//(new CMaterialVulkan());
    return material;
}

IMeshPtr CRendererVulkan::CreateMesh()
{
    IMeshPtr mesh(new CMeshVulkan());
    return mesh;
}

IShaderPtr CRendererVulkan::CreateShader()
{
    IShaderPtr shader(new CShaderVulkan(Ptr<CRendererVulkan>()));
    return shader;
}

IShaderProgramPtr CRendererVulkan::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram;//(new CShaderProgramVulkan());
    return shaderProgram;
}

void CRendererVulkan::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    /*assert(mesh && material);
    IVertexBufferPtr vertexBuffer = mesh->VertexBuffer();
    IIndexBufferPtr indexBuffer = mesh->IndexBuffer();

    if (indexBuffer) {
        Draw(vertexBuffer, indexBuffer, material);
    } else {
        Draw(vertexBuffer, material);
    }*/
}

void CRendererVulkan::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer || !material) {
        return;
    }

    //int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    //glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererVulkan::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer || !indexBuffer || !material) {
        return;
    }

    //int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    //glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, nullptr);
}

const VkDevice& CRendererVulkan::LogicalDevice() const
{
    return m_LogicalDevice;
}

const VkQueue& CRendererVulkan::Queue() const
{
    return m_Queue;
}

const std::vector<VkCommandBuffer>& CRendererVulkan::CommandBuffers() const
{
    return m_CommandBuffers;
}

const VkCommandBuffer& CRendererVulkan::CommandBuffer(uint32_t swapchainIndex) const
{
    assert(swapchainIndex < m_CommandBuffers.size() && "Incorrect swapchain index");
    return m_CommandBuffers[swapchainIndex];
}

const std::vector<VkImage>& CRendererVulkan::SwapchainImages() const
{
    return m_SwapchainImages;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

std::vector<VkQueueFamilyProperties> CRendererVulkan::GetPhysicalDeviceQueueProps(const VkInstance& instance,
                                                                                  const VkPhysicalDevice physicalDevice)
{
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueProperties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, &queueProperties[0]);
    
    return queueProperties;
}

std::tuple<VkResult, VkDevice> CRendererVulkan::CreateLogicalDevice(const VkPhysicalDevice& physicalDevice /* debug layer */)
{
    float queuePriorities[1] = { 0.0 };
    VkDeviceQueueCreateInfo queueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = queuePriorities,
    };
    
    VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = nullptr
    };
    
    VkDevice logicalDevice;
    VkResult result = vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &logicalDevice);
    
    std::tuple<VkResult, VkDevice> logicalDeviceData(result, logicalDevice);
    return logicalDeviceData;
}

std::tuple<bool, VkQueueFamilyProperties, uint32_t> CRendererVulkan::FindQueueProperties(VkQueueFlags flag,
                                                                                         const std::vector<VkBool32>& supportsPresent)
{
    std::tuple<bool, VkQueueFamilyProperties, uint32_t> queueProps;
    std::get<0>(queueProps) = false;
    
    int index = 0;
    std::all_of(m_QueueProperties.begin(), m_QueueProperties.end(), [&](VkQueueFamilyProperties props) {
        if (index < supportsPresent.size() && !supportsPresent[index]) {
            ++index;
            return false;
        }
        
        if (props.queueFlags & flag) {
            std::get<0>(queueProps) = true;
            std::get<1>(queueProps) = props;
            std::get<2>(queueProps) = index;
            return true;
        }
        ++index;
        
        return false;
    });
    return queueProps;
}

//#endif /* defined(RENDER_VULKAN) */
