//
//  CIndexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CIndexBufferVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferVulkan::CIndexBufferVulkan(const VkDevice& logicalDevice)
: m_ElementSize(0)
, m_Size(0)
, m_LogicalDevice(logicalDevice)
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
    
    if (m_Buffer) {
        vkDestroyBuffer(m_LogicalDevice, m_Buffer, nullptr);
        m_Buffer = nullptr;
    }
    
    if (m_DeviceMemory) {
        vkFreeMemory(m_LogicalDevice, m_DeviceMemory, nullptr);
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
    
    VkResult result = vkCreateBuffer(m_LogicalDevice, &vk_bufferCreateInfo, nullptr, &m_Buffer);
    if (result != VK_SUCCESS) {
        JAM_LOG("Can't create Vulkan index buffer");
    }
    
    VkMemoryRequirements vk_memoryRequirements;
    vkGetBufferMemoryRequirements(m_LogicalDevice, m_Buffer, &vk_memoryRequirements);
    
    /*for(size_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
    {
        auto bit = ((uint32_t)1 << i);
        if((vk_memoryRequirements.memoryTypeBits & bit) != 0)
        {
            if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
            {
                return i;
            }
        }
    }*/
    
    VkMemoryAllocateInfo vk_memoryAllocateInfo = {
        vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        vk_memoryAllocateInfo.pNext = nullptr,
        vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size,
        vk_memoryAllocateInfo.memoryTypeIndex = 0 // TODO:
    };
    
    result = vkAllocateMemory(m_LogicalDevice, &vk_memoryAllocateInfo, nullptr, &m_DeviceMemory);
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
    
    VkResult result = vkMapMemory(m_LogicalDevice, m_DeviceMemory, 0, Size(), 0, &m_MappedData);
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
    
    vkUnmapMemory(m_LogicalDevice, m_DeviceMemory);
    m_MappedData = nullptr;
    
    VkResult result = vkBindBufferMemory(m_LogicalDevice, m_Buffer, m_DeviceMemory, 0);
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
