//
//  CRenderTargetDepthVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
//#if defined(RENDER_VULKAN)

#include "CRenderTargetDepthVulkan.h"
#include "CRenderTargetStencilVulkan.h"
#include "CRendererVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthVulkan::CRenderTargetDepthVulkan(CRendererVulkanPtr renderer)
: m_Renderer(renderer)
, m_IsInitialized(false)
{
}

CRenderTargetDepthVulkan::~CRenderTargetDepthVulkan()
{
    
}

void CRenderTargetDepthVulkan::Initialize(InternalFormats internalFormat)
{
    if (IsInitialized()) {
        return;
    }
    m_Format = ConvertInternalFormat(internalFormat);
    m_IsInitialized = true;
}

void CRenderTargetDepthVulkan::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }
    m_IsInitialized = false;
}

bool CRenderTargetDepthVulkan::IsInitialized()
{
    return m_IsInitialized;
}

void CRenderTargetDepthVulkan::Allocate(uint64_t width, uint64_t height)
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
            .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };
        
        m_Images.resize(1);
        VkResult result = vkCreateImage(renderer->LogicalDevice(), &imageCreateInfo, nullptr, m_Images.data());
        if (result != VK_SUCCESS) {
            m_Images.clear();
            return;
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
            JAM_LOG("CRenderTargetColorVulkan::Allocate - Error creating image view");
        }
    }
}

void CRenderTargetDepthVulkan::Bind() const
{
}

void CRenderTargetDepthVulkan::Unbind() const
{
}

const std::vector<VkImage>& CRenderTargetDepthVulkan::Images() const
{
    return m_Images;
}

const std::vector<VkImageView>& CRenderTargetDepthVulkan::ImageViews() const
{
    return m_ImageViews;
}

VkFormat CRenderTargetDepthVulkan::ConvertInternalFormat(IRenderTarget::InternalFormats internalFormat)
{
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    switch (internalFormat) {
        case Depth16:
            format = VK_FORMAT_D16_UNORM;
            break;
        case Depth24:
            format = VK_FORMAT_D24_UNORM_S8_UINT;
            break;
        case Depth32:
            format = VK_FORMAT_D32_SFLOAT;
            break;
        case Depth24_Stencil8:
            format = VK_FORMAT_D24_UNORM_S8_UINT;
            break;

        default:
            break;
    }
    return format;
}

IRenderTarget::InternalFormats CRenderTargetDepthVulkan::ConvertInternalFormat(VkFormat vkFormat)
{
    IRenderTarget::InternalFormats internalFormat = ColorRGBA8888;
    switch (vkFormat) {
        case VK_FORMAT_D16_UNORM:
            internalFormat = Depth16;
            break;
        case VK_FORMAT_D32_SFLOAT:
            internalFormat = Depth32;
            break;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            internalFormat = Depth24_Stencil8;
            break;
            
        default:
            break;
    }
    return internalFormat;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
