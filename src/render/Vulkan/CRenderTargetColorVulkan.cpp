//
//  CRenderTargetColorVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
//#if defined(RENDER_VULKAN)

#include "CRenderTargetColorVulkan.h"
#include "CRendererVulkan.h"
#include "IRenderView.h"
#include "CRenderInstanceVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorVulkan::CRenderTargetColorVulkan(CRendererVulkanPtr renderer)
: m_IsInitialized(false)
, m_Renderer(renderer)
{
}

CRenderTargetColorVulkan::~CRenderTargetColorVulkan()
{
}

void CRenderTargetColorVulkan::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized()) {
        return;
    }
    m_Format = ConvertInternalFormat(internalFormat);
    m_IsInitialized = true;
}

void CRenderTargetColorVulkan::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }
    m_IsInitialized = false;
}

bool CRenderTargetColorVulkan::IsInitialized()
{
    return m_IsInitialized;
}

void CRenderTargetColorVulkan::Allocate(uint64_t width, uint64_t height)
{
    CRendererVulkanPtr renderer = m_Renderer.lock();
    if (m_Images.size() == 0) {
        const VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = m_Format,
            .extent = {
                .width = (uint32_t)width,
                .height = (uint32_t)height,
                .depth = 1
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };
        
        m_Images.resize(1);
        VkResult result = vkCreateImage(renderer->LogicalDevice(), &imageCreateInfo, nullptr, m_Images.data());
        if (result != VK_SUCCESS) {
            m_Images.clear();
            return;
        }
        
        for (size_t i = 0; i < m_Images.size(); ++i) {
            VkMemoryRequirements vk_memoryRequirements;
            vkGetImageMemoryRequirements(renderer->LogicalDevice(), m_Images[i], &vk_memoryRequirements);
            CRenderInstanceVulkanPtr instance = renderer->RenderView()->RenderInstance()->Ptr<CRenderInstanceVulkan>();
            
            VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
            vkGetPhysicalDeviceMemoryProperties(instance->PhysicalDevice(), &vk_physicalDeviceMemoryProperties);
            
            uint32_t memoryDeviceIndex = UINT32_MAX;
            for(uint32_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
            {
                auto bit = ((uint32_t)1 << i);
                if((vk_memoryRequirements.memoryTypeBits & bit) != 0)
                {
                    if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0)
                    {
                        memoryDeviceIndex = i;
                        break;
                    }
                }
            }
            
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.pNext = nullptr;
            allocInfo.allocationSize = vk_memoryRequirements.size;
            allocInfo.memoryTypeIndex = memoryDeviceIndex;
            
            VkDeviceMemory imageMemory;
            result = vkAllocateMemory(renderer->LogicalDevice(), &allocInfo, nullptr, &imageMemory);
            if (result != VK_SUCCESS) {
                JAM_LOG("failed to allocate color image memory!");
                m_Images.clear();
                return;
            }
            
            vkBindImageMemory(renderer->LogicalDevice(), m_Images[i], imageMemory, 0);
        }
    }
    
    std::for_each(m_ImageViews.begin(), m_ImageViews.end(), [&](const VkImageView& imageView) {
        vkDestroyImageView(renderer->LogicalDevice(), imageView, nullptr);
    });
    
    m_ImageViews.resize(m_Images.size());
    for (uint32_t i = 0; i < m_Images.size(); i++) {
        VkImageViewCreateInfo imageViewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .format = m_Format,
            .image = m_Images[i],
            .components = {
                .r = VK_COMPONENT_SWIZZLE_R,
                .g = VK_COMPONENT_SWIZZLE_G,
                .b = VK_COMPONENT_SWIZZLE_B,
                .a = VK_COMPONENT_SWIZZLE_A,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            },
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .flags = 0,
        };
        
        VkResult result = vkCreateImageView(renderer->LogicalDevice(),
                                            &imageViewInfo,
                                            nullptr,
                                            &m_ImageViews[i]);
        if (result != VK_SUCCESS) {
            JAM_LOG("CRenderTargetColorVulkan::Allocate - Error creating image view")
        }
    }
}

void CRenderTargetColorVulkan::Bind() const
{
}

void CRenderTargetColorVulkan::Unbind() const
{
}

IRenderTarget::InternalFormats CRenderTargetColorVulkan::InternalFormat() const
{
    return ConvertInternalFormat(m_Format);
}

void CRenderTargetColorVulkan::InitializeWithImages(const std::vector<VkImage>& images,
                                                    uint64_t width, uint64_t height)
{
    m_Images = images;
    Allocate(width, height);
}

const std::vector<VkImage>& CRenderTargetColorVulkan::Images() const
{
    return m_Images;
}

const std::vector<VkImageView>& CRenderTargetColorVulkan::ImageViews() const
{
    return m_ImageViews;
}

VkFormat CRenderTargetColorVulkan::ConvertInternalFormat(IRenderTarget::InternalFormats internalFormat)
{
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    switch (internalFormat) {
        case ColorR8:
            format = VK_FORMAT_R8_UNORM;
            break;
        case ColorR16:
            format = VK_FORMAT_R16_UNORM;
            break;
        case ColorR32:
            format = VK_FORMAT_R32_SFLOAT;
            break;
        case ColorRGB888:
            format = VK_FORMAT_R8G8B8_UNORM;
            break;
        case ColorRGB565:
            format = VK_FORMAT_R5G6B5_UNORM_PACK16;
            break;
        case ColorRGBA8888:
            format = VK_FORMAT_R8G8B8A8_UNORM;
            break;
        case ColorRGBA4444:
            format = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
            break;
        case ColorRGB10_A2:
            format = VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            break;
            
        default:
            break;
    }
    return format;
}

IRenderTarget::InternalFormats CRenderTargetColorVulkan::ConvertInternalFormat(VkFormat vkFormat)
{
    IRenderTarget::InternalFormats internalFormat = ColorRGBA8888;
    switch (vkFormat) {
        case VK_FORMAT_R8_UNORM:
            internalFormat = ColorR8;
            break;
        case VK_FORMAT_R16_UNORM:
            internalFormat = ColorR16;
            break;
        case VK_FORMAT_R32_SFLOAT:
            internalFormat = ColorR32;
            break;
        case VK_FORMAT_R8G8B8_UNORM:
            internalFormat = ColorRGB888;
            break;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            internalFormat = ColorRGB565;
            break;
        case VK_FORMAT_R8G8B8A8_UNORM:
            internalFormat = ColorRGBA8888;
            break;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            internalFormat = ColorRGBA4444;
            break;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            internalFormat = ColorRGB10_A2;
            break;
            
        default:
            break;
    }
    return internalFormat;
}

VkFormat CRenderTargetColorVulkan::Format() const
{
    return m_Format;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
