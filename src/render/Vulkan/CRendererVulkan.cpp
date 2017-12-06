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
{
    CRenderInstanceVulkanPtr instance = RenderView()->GetRenderInstance<CRenderInstanceVulkan>();
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
}

CRendererVulkan::~CRendererVulkan()
{
}

void CRendererVulkan::Initialize()
{
    CFrameBufferVulkanPtr frameBuffer = std::static_pointer_cast<CFrameBufferVulkan>(RenderView()->DefaultFrameBuffer());
    uint32_t count = (uint32_t)frameBuffer->FrameBuffers().size();
    const VkCommandBufferAllocateInfo cmd = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = m_CommandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = count,
    };
    
    m_CommandBuffers.resize(count);
    VkResult result = vkAllocateCommandBuffers(m_LogicalDevice, &cmd, &m_CommandBuffers[0]);
    assert(result == VK_SUCCESS);
    
    const VkRenderPass& renderPass = frameBuffer->RenderPass();
    for (uint32_t i = 0; i < frameBuffer->FrameBuffers().size(); ++i) {
        const VkFramebuffer& fb = frameBuffer->FrameBuffers()[i];
        
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
            .pInheritanceInfo = nullptr //&commandBufferInheritanceInfo
        };
        
        vkBeginCommandBuffer(m_CommandBuffers[i], &commandBufferBeginInfo);
        {
            VkClearValue clearValue = {
                .color.float32[0] = 1.0f,
                .color.float32[1] = 0.0f,
                .color.float32[2] = 0.0f,
                .color.float32[3] = 1.0f
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
            
            vkCmdBeginRenderPass(m_CommandBuffers[i],
                                 &renderPassBeginInfo,
                                 VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
            //vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pip);
            vkCmdEndRenderPass(m_CommandBuffers[i]);
            
            CRenderTargetColorVulkanPtr colorTarget = std::static_pointer_cast<CRenderTargetColorVulkan>(frameBuffer->ColorAttachement(0));
            
            VkImageMemoryBarrier imageMemoryBarrier = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .pNext = nullptr,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = colorTarget->Images()[i],
                .subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            };
            
            vkCmdPipelineBarrier(m_CommandBuffers[i],
                                 VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                                 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &imageMemoryBarrier);
        }
        vkEndCommandBuffer(m_CommandBuffers[i]);
    }
}

IRenderViewPtr CRendererVulkan::RenderView() const
{
    return m_RenderView;
}

IFrameBufferPtr CRendererVulkan::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    CRenderInstanceVulkanPtr instance = RenderView()->GetRenderInstance<CRenderInstanceVulkan>();
    IFrameBufferPtr frameBuffer(new CFrameBufferVulkan(width, height,
                                                       m_LogicalDevice,
                                                       instance));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererVulkan::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorVulkan(m_LogicalDevice));
    return colorTarget;
}

CRenderTargetDepthPtr CRendererVulkan::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget;//(new CRenderTargetDepthVulkan());
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
    IVertexBufferPtr vertexBuffer;//(new CVertexBufferVulkan());
    return vertexBuffer;
}

IIndexBufferPtr CRendererVulkan::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer;//(new CIndexBufferVulkan());
    return indexBuffer;
}

ITexturePtr CRendererVulkan::CreateTexture()
{
    ITexturePtr texture;//(new CTextureVulkan());
    return texture;
}

IMaterialPtr CRendererVulkan::CreateMaterial()
{
    IMaterialPtr material;//(new CMaterialVulkan());
    return material;
}

IMeshPtr CRendererVulkan::CreateMesh()
{
    IMeshPtr mesh;//(new CMeshVulkan());
    return mesh;
}

IShaderPtr CRendererVulkan::CreateShader()
{
    IShaderPtr shader;//(new CShaderVulkan());
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
