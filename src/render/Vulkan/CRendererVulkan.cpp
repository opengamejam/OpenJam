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
    
    // GPU Queue Props
    {
        m_QueueProperties = GetPhysicalDeviceQueueProps(instance->Instance(), instance->PhysicalDevice());
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
}

CRendererVulkan::~CRendererVulkan()
{
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

//#endif /* defined(RENDER_VULKAN) */
