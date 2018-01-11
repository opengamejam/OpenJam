//
//  CIndexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CIndexBufferVulkan.h"
#include "CRendererVulkan.h"
#include "CRenderInstanceVulkan.h"
#include "IRenderView.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferVulkan::CIndexBufferVulkan(CRendererVulkanPtr renderer)
: m_ElementSize(0)
, m_Size(0)
, m_Renderer(renderer)
, m_Buffer(nullptr)
, m_DeviceMemory(nullptr)
, m_MappedData(nullptr)
{
}

CIndexBufferVulkan::~CIndexBufferVulkan()
{
}

void CIndexBufferVulkan::Initialize(DataTypes dataType)
{
    m_Stream = IIndexBuffer::SIndexStream(Ptr<IIndexBuffer>());
    m_Stream.dataType = dataType;
    ElementSize(IIndexBuffer::SIndexStream::SizeForType(dataType));
}

void CIndexBufferVulkan::Shutdown()
{
}

bool CIndexBufferVulkan::IsValid() const
{
    return true;
}

uint64_t CIndexBufferVulkan::SizeRaw() const
{
    return m_Size;
}

void CIndexBufferVulkan::ResizeRaw(uint64_t newSize)
{
    m_Size = newSize;
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    CRenderInstanceVulkanPtr instance = renderer->RenderView()->RenderInstance()->Ptr<CRenderInstanceVulkan>();
    
    VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(instance->PhysicalDevice(), &vk_physicalDeviceMemoryProperties);
    
    if (m_Buffer) {
        vkDestroyBuffer(renderer->LogicalDevice(), m_Buffer, nullptr);
        m_Buffer = nullptr;
    }
    
    if (m_DeviceMemory) {
        vkFreeMemory(renderer->LogicalDevice(), m_DeviceMemory, nullptr);
        m_DeviceMemory = nullptr;
    }
    
    VkBufferCreateInfo vk_bufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = newSize,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };
    
    VkResult result = vkCreateBuffer(renderer->LogicalDevice(), &vk_bufferCreateInfo, nullptr, &m_Buffer);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create Vulkan index buffer");
    }
    
    VkMemoryRequirements vk_memoryRequirements;
    vkGetBufferMemoryRequirements(renderer->LogicalDevice(), m_Buffer, &vk_memoryRequirements);
    
    uint32_t memoryDeviceIndex = UINT32_MAX;
    for(uint32_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
    {
        auto bit = ((uint32_t)1 << i);
        if((vk_memoryRequirements.memoryTypeBits & bit) != 0)
        {
            if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                memoryDeviceIndex = i;
            }
        }
    }
    
    if (memoryDeviceIndex == UINT32_MAX) {
        JAM_LOG("Can't find appropriate Vulkan device memory for index buffer");
        return;
    }
    
    VkMemoryAllocateInfo vk_memoryAllocateInfo = {
        vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        vk_memoryAllocateInfo.pNext = nullptr,
        vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size,
        vk_memoryAllocateInfo.memoryTypeIndex = memoryDeviceIndex
    };
    
    result = vkAllocateMemory(renderer->LogicalDevice(), &vk_memoryAllocateInfo, nullptr, &m_DeviceMemory);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't allocate Vulkan device memory for index buffer");
    }
}

uint64_t CIndexBufferVulkan::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferVulkan::LockRaw()
{
    if (IsLocked()) {
        return m_MappedData;
    }
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    assert(renderer);
    
    VkResult result = vkMapMemory(renderer->LogicalDevice(), m_DeviceMemory, 0, Size(), 0, &m_MappedData);
    if (result == VK_SUCCESS) {
        JAM_LOG("Can't map device memory for index buffer")
    }
    
    return m_MappedData;
}

IIndexBuffer::SIndexStream& CIndexBufferVulkan::Lock()
{
    return m_Stream;
}

bool CIndexBufferVulkan::IsLocked() const
{
    return m_MappedData != nullptr;
}

void CIndexBufferVulkan::Unlock(bool isNeedCommit)
{
    if (!IsLocked()) {
        return;
    }
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    assert(renderer);
    
    vkUnmapMemory(renderer->LogicalDevice(), m_DeviceMemory);
    m_MappedData = nullptr;
    
    VkResult result = vkBindBufferMemory(renderer->LogicalDevice(), m_Buffer, m_DeviceMemory, 0);
    if (result == VK_SUCCESS) {
        JAM_LOG("Can't bind index buffer with device memory")
    }
}

void CIndexBufferVulkan::Bind()
{
}

void CIndexBufferVulkan::Unbind()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferVulkan::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
