//
//  CTextureVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CTextureVulkan.h"
#include "IImage.h"
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

CTextureVulkan::CTextureVulkan(CRendererVulkanPtr renderer)
: m_Filter(ITexture::Linear)
, m_IsDirty(true)
, m_Renderer(renderer)
, m_Image(nullptr)
, m_DeviceMemory(nullptr)
, m_ImageView(nullptr)
, m_Sampler(nullptr)
{
}

CTextureVulkan::~CTextureVulkan()
{
}

void CTextureVulkan::Bind()
{
}

void CTextureVulkan::Unbind()
{
}

void CTextureVulkan::Filter(ITexture::TextureFilters filter)
{
    m_Filter = filter;
    m_IsDirty = true;
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    
    if (m_Sampler) {
        vkDestroySampler(renderer->LogicalDevice(), m_Sampler, nullptr);
        m_Sampler = nullptr;
    }
    
    VkFilter vkFilter = VK_FILTER_LINEAR;
    if (filter == Nearest) {
        vkFilter = VK_FILTER_NEAREST;
    }
    
    VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = vkFilter,
        .minFilter = vkFilter,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = 16,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR
    };
    
    VkResult result = vkCreateSampler(renderer->LogicalDevice(), &samplerInfo, nullptr, &m_Sampler);
    if (result != VK_SUCCESS) {
        JAM_LOG("failed to create texture sampler!");
    }
}

ITexture::TextureFilters CTextureVulkan::Filter() const
{
    return m_Filter;
}

void CTextureVulkan::Allocate(uint64_t width, uint64_t height)
{
    // TODO:
    
    Filter(ITexture::TextureFilters::Linear);
}

bool CTextureVulkan::AssignImage(IImagePtr image)
{
    assert(image && !image->RawData().empty());
    if (!image) {
        return false;
    }
    
    CRendererVulkanPtr renderer = m_Renderer.lock();
    CRenderInstanceVulkanPtr instance = renderer->RenderView()->RenderInstance()->Ptr<CRenderInstanceVulkan>();
    
    VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(instance->PhysicalDevice(), &vk_physicalDeviceMemoryProperties);
    
    if (m_Image) {
        vkDestroyImage(renderer->LogicalDevice(), m_Image, nullptr);
        m_Image = nullptr;
    }
    if (m_DeviceMemory) {
        vkFreeMemory(renderer->LogicalDevice(), m_DeviceMemory, nullptr);
        m_DeviceMemory = nullptr;
    }
    if (m_ImageView) {
        vkDestroyImageView(renderer->LogicalDevice(), m_ImageView, nullptr);
        m_ImageView = nullptr;
    }
    
    VkFormat vkFormat = VK_FORMAT_R8G8B8A8_UNORM; // TODO: image->TexelFormat();
    const VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = vkFormat,
        .extent = {
            .width = (uint32_t)image->Width(),
            .height = (uint32_t)image->Height(),
            .depth = 1
        },
        .mipLevels = 1, // TODO: image->MipsCount()
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    
    VkResult result = vkCreateImage(renderer->LogicalDevice(), &imageCreateInfo, nullptr, &m_Image);
    if (result != VK_SUCCESS) {
        return false;
    }
    
    VkMemoryRequirements vk_memoryRequirements;
    vkGetImageMemoryRequirements(renderer->LogicalDevice(), m_Image, &vk_memoryRequirements);
    
    uint32_t memoryDeviceIndex = UINT32_MAX;
    for(uint32_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
    {
        auto bit = ((uint32_t)1 << i);
        if((vk_memoryRequirements.memoryTypeBits & bit) != 0)
        {
            VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) != 0)
            {
                memoryDeviceIndex = i;
                break;
            }
        }
    }
    
    VkMemoryAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = vk_memoryRequirements.size,
        .memoryTypeIndex = memoryDeviceIndex
    };
    
    result = vkAllocateMemory(renderer->LogicalDevice(), &allocInfo, nullptr, &m_DeviceMemory);
    if (result != VK_SUCCESS) {
        JAM_LOG("failed to allocate texture image memory!");
        return false;
    }
    
    vkBindImageMemory(renderer->LogicalDevice(), m_Image, m_DeviceMemory, 0);
    
    void* data;
    vkMapMemory(renderer->LogicalDevice(), m_DeviceMemory, 0, image->RawData().size(), 0, &data);
    memcpy(data, image->RawData().data(), image->RawData().size());
    vkUnmapMemory(renderer->LogicalDevice(), m_DeviceMemory);
    
    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_Image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = vkFormat,
        .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel = 0,
        .subresourceRange.levelCount = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount = 1
    };
    
    result = vkCreateImageView(renderer->LogicalDevice(), &viewInfo, nullptr, &m_ImageView);
    if (result != VK_SUCCESS) {
        JAM_LOG("failed to create texture image view!");
        return false;
    }
    
    if (image->MipsCount() > 1) {
        Filter(ITexture::TextureFilters::UseMipMaps);
    } else {
        Filter(ITexture::TextureFilters::Linear);
    }
    
    return true;
}

const std::string& CTextureVulkan::Hash()
{
    if (m_IsDirty) {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CTextureVulkan::HashMe()
{
    std::stringstream ss;
    ss << Filter();
    
    m_Hash = ss.str();
}


// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
