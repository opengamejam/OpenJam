//
//  CRenderTargetColorVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
//#if defined(RENDER_VULKAN)

#include "CRenderTargetColorVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorVulkan::CRenderTargetColorVulkan(VkDevice logicalDevice)
: m_LogicalDevice(logicalDevice)
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
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };
        VkImage image;
        vkCreateImage(m_LogicalDevice, &imageCreateInfo, nullptr, &image);
        m_Images.push_back(image);
    }
    
    std::for_each(m_ImageViews.begin(), m_ImageViews.end(), [&](const VkImageView& imageView) {
        vkDestroyImageView(m_LogicalDevice, imageView, nullptr);
    });
    
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
        
        VkImageView imageView;
        VkResult result = vkCreateImageView(m_LogicalDevice,
                                            &imageViewInfo,
                                            nullptr,
                                            &imageView);
        if (result == VK_SUCCESS) {
            m_ImageViews.push_back(imageView);
        } else {
            JAM_LOG("CRenderTargetColorVulkan::Allocate - Error creating image view")
            break;
        }
    }
}

void CRenderTargetColorVulkan::Bind() const
{
}

void CRenderTargetColorVulkan::Unbind() const
{
}

void CRenderTargetColorVulkan::InitializeWithImages(const std::vector<VkImage>& images, uint64_t width, uint64_t height)
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

VkFormat CRenderTargetColorVulkan::ConvertInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
        case ColorR8:
            return VK_FORMAT_R8_UNORM;
            break;
        case ColorR16:
            return VK_FORMAT_R16_UNORM;
            break;
        case ColorR32:
            return VK_FORMAT_R32_UINT;
            break;
        case ColorRGB888:
            return VK_FORMAT_R8G8B8_UINT;
            break;
        case ColorRGB565:
            return VK_FORMAT_R5G6B5_UNORM_PACK16;
            break;
        case ColorRGBA8888:
            return VK_FORMAT_R8G8B8A8_UNORM;
            break;
        case ColorRGBA4444:
            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
            break;
        case ColorRGB10_A2:
            return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            break;
        case Depth16:
            return VK_FORMAT_D16_UNORM;
            break;
        case Depth24:
            return VK_FORMAT_D24_UNORM_S8_UINT;
            break;
        case Depth32:
            return VK_FORMAT_D32_SFLOAT;
            break;
        case Stencil8:
            return VK_FORMAT_S8_UINT;
            break;
        case Depth24_Stencil8:
            return VK_FORMAT_D24_UNORM_S8_UINT;
            break;
            
        default:
            break;
    }
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
