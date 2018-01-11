//
//  CVertexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CVertexBufferVulkan.h"
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

CVertexBufferVulkan::CVertexBufferVulkan(CRendererVulkanPtr renderer)
: m_ElementSize(0)
, m_ZeroStride(false)
, m_Size(0)
, m_Renderer(renderer)
, m_Buffer(nullptr)
, m_DeviceMemory(nullptr)
, m_MappedData(nullptr)
{
    
}

CVertexBufferVulkan::~CVertexBufferVulkan()
{
}

void CVertexBufferVulkan::Initialize(uint64_t elementSize)
{
    ElementSize(elementSize);
    
    
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferVulkan::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferVulkan::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end()) {
        uint64_t absoluteOffset = 0;
        std::for_each(m_VertexStreamers.begin(), m_VertexStreamers.end(), [&](const TVertexStreamMap::value_type& value) {
            const IVertexBuffer::SVertexStream& stream = value.second;
            absoluteOffset += (stream.DataSize() * Size());
        });
        
        SVertexStream stream = SVertexStream(Ptr<IVertexBuffer>());
        stream.streamIndex = (unsigned int)m_VertexStreamers.size();
        stream.absoluteOffset = absoluteOffset;
        
        m_VertexStreamers[vertexType] = stream;
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferVulkan::Shutdown()
{
}

bool CVertexBufferVulkan::IsValid() const
{
    return true;
}

uint64_t CVertexBufferVulkan::SizeRaw() const
{
    return m_Size;
}

void CVertexBufferVulkan::ResizeRaw(uint64_t newSize)
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
        JAM_LOG("Can't create Vulkan vertex buffer");
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
        JAM_LOG("Can't find appropriate Vulkan device memory for vertex buffer");
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
        JAM_LOG("Can't allocate Vulkan device memory for vertex buffer");
    }
}

uint64_t CVertexBufferVulkan::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferVulkan::LockRaw()
{
    if (IsLocked()) {
        return m_MappedData;
    }
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    
    VkResult result = vkMapMemory(renderer->LogicalDevice(), m_DeviceMemory, 0, Size(), 0, &m_MappedData);
    if (result == VK_SUCCESS) {
        JAM_LOG("Can't map device memory for vertex buffer")
    }
    
    return m_MappedData;
}

bool CVertexBufferVulkan::IsLocked() const
{
    return m_MappedData != nullptr;
}

void CVertexBufferVulkan::Unlock(bool isNeedCommit)
{
    if (!IsLocked()) {
        return;
    }
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    
    vkUnmapMemory(renderer->LogicalDevice(), m_DeviceMemory);
    m_MappedData = nullptr;
    
    VkResult result = vkBindBufferMemory(renderer->LogicalDevice(), m_Buffer, m_DeviceMemory, 0);
    if (result == VK_SUCCESS) {
        JAM_LOG("Can't bind vertex buffer with device memory")
    }
}

bool CVertexBufferVulkan::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferVulkan::ZeroStride(bool isZeroStride)
{
    m_ZeroStride = isZeroStride;
}

bool CVertexBufferVulkan::ZeroStride()
{
    return m_ZeroStride;
}

void CVertexBufferVulkan::Bind()
{
}

void CVertexBufferVulkan::Unbind()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferVulkan::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
