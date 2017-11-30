//
//  CFrameBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CFrameBufferVulkan.h"
#include "CRenderTargetColorVulkan.h"
#include "CRenderTargetDepthVulkan.h"
#include "CRenderTargetStencilVulkan.h"
#include "CRenderInstanceVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferVulkan::CFrameBufferVulkan(uint32_t width, uint32_t height,
                                       const VkDevice& logicalDevice,
                                       CRenderInstanceVulkanPtr renderInstance)
: m_IsInitialized(false)
, m_Width(width)
, m_Height(height)
, m_LogicalDevice(logicalDevice)
, m_RenderInstance(renderInstance)
{
}

CFrameBufferVulkan::~CFrameBufferVulkan()
{
}

void CFrameBufferVulkan::Initialize()
{
    if (IsInitialized()) {
        return;
    }
    
    m_IsInitialized = true;
}

void CFrameBufferVulkan::Shutdown()
{
    if (!IsInitialized()) {
        return;
    }
    
    m_IsInitialized = false;
}

bool CFrameBufferVulkan::IsInitialized()
{
    return m_IsInitialized;
}

void CFrameBufferVulkan::Resize(uint64_t width, uint64_t height)
{
    m_Width = width;
    m_Height = height;
    
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [width, height](IRenderTargetPtr renderTarget)
    {
        if (renderTarget)
        {
            renderTarget->Allocate(width, height);
        }
    });
    
    if (DepthAttachement()) {
       // std::static_pointer_cast<CRenderTargetDepthVulkan>(DepthAttachement())->Allocate(width, height);
    }
    
    if (StencilAttachement()) {
     //   std::static_pointer_cast<CRenderTargetStencilVulkan>(StencilAttachement())->Allocate(width, height);
    }

    JAM_LOG("CFrameBufferVulkan::Resize() - id: %d, size: (%llu, %llu)\n", m_Id, width, height);
}

uint64_t CFrameBufferVulkan::MaxColorAttachements() const
{
    return m_NumColorAtachments;
}

void CFrameBufferVulkan::AttachColor(IRenderTargetPtr colorTarget, uint64_t index)
{
    if (!colorTarget) {
        return;
    }
    
    if (index >= m_ColorBuffers.size()) {
        m_ColorBuffers.resize(index);
    }
    m_ColorBuffers[index] = colorTarget;
    Rebuild();
}

void CFrameBufferVulkan::AttachDepth(IRenderTargetPtr depthTarget)
{
    if (!depthTarget) {
        return;
    }
    m_DepthBuffer = depthTarget;
    Rebuild();
}

void CFrameBufferVulkan::AttachStencil(IRenderTargetPtr stencilTarget)
{
    if (!stencilTarget) {
        return;
    }
    m_StencilBuffer = stencilTarget;
    Rebuild();
}

void CFrameBufferVulkan::DetachColor(uint64_t index)
{
    if (index < m_ColorBuffers.size()) {
        m_ColorBuffers[index] = nullptr;
    }
}

void CFrameBufferVulkan::DetachDepth()
{
    m_DepthBuffer = nullptr;
}

void CFrameBufferVulkan::DetachStencil()
{
    m_StencilBuffer = nullptr;
}

IRenderTargetPtr CFrameBufferVulkan::ColorAttachement(uint64_t index) const
{
    if (index < MaxColorAttachements()) {
        return m_ColorBuffers[(size_t)index];
    }
    return nullptr;
}

IRenderTargetPtr CFrameBufferVulkan::DepthAttachement() const
{
    return m_DepthBuffer;
}

IRenderTargetPtr CFrameBufferVulkan::StencilAttachement() const
{
    if (DepthAttachement() && !m_StencilBuffer) {
        DepthAttachement()->StencilTarget();
    }
    return m_StencilBuffer;
}

bool CFrameBufferVulkan::IsValid() const
{
    return m_Framebuffers.size() > 0; // TODO
}

void CFrameBufferVulkan::Bind() const
{
}

void CFrameBufferVulkan::Unbind() const
{
}

void CFrameBufferVulkan::Clear() const
{
}

void CFrameBufferVulkan::ClearColor(const CColor4f& color)
{
    m_ClearColor = color;
}

const CColor4f& CFrameBufferVulkan::ClearColor() const
{
    return m_ClearColor;
}

uint64_t CFrameBufferVulkan::Width() const
{
    return m_Width;
}

uint64_t CFrameBufferVulkan::Height() const
{
    return m_Height;
}

IFrameBuffer::TRawData CFrameBufferVulkan::RawData()
{
    uint64_t rawdataSize = Width() * Height() * 4;
    IFrameBuffer::TRawData data((size_t)rawdataSize, 0);
    
    // TODO
    
    return data;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CFrameBufferVulkan::Rebuild()
{
    CRenderInstanceVulkanPtr renderInstance = m_RenderInstance.lock();
    if (!renderInstance) {
        JAM_LOG("CFrameBufferVulkan::Rebuild - Cannot rebuild framebuffer without render instance");
        return;
    }
    const std::vector<VkSurfaceFormatKHR>& surfaceFormats = renderInstance->SurfaceFormats();
    
    if (m_RenderPass) {
        vkDestroyRenderPass(m_LogicalDevice, m_RenderPass, nullptr);
    }
    std::for_each(m_Framebuffers.begin(), m_Framebuffers.end(), [&](const VkFramebuffer& framebuffer) {
        vkDestroyFramebuffer(m_LogicalDevice, framebuffer, nullptr);
    });
    m_Framebuffers.clear();
    
    std::vector<IRenderTargetPtr> renderTargets(m_ColorBuffers);
    renderTargets.push_back(m_DepthBuffer);
    
    std::vector<VkImageView> imageViews;
    std::vector<VkAttachmentDescription> attachementDescription;
    std::for_each(renderTargets.begin(), renderTargets.end(), [&](IRenderTargetPtr renderTarget) {
        if (renderTarget) {
            VkAttachmentDescription desc = {
                .flags = 0,
                .format = surfaceFormats[0].format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };
            attachementDescription.push_back(desc);
            
            if (renderTarget->ColorTarget()) {
                const std::vector<VkImageView>& views = std::static_pointer_cast<CRenderTargetColorVulkan>(renderTarget)->ImageViews();
                std::for_each(views.begin(), views.end(), [&](const VkImageView& imageView){
                    imageViews.push_back(imageView);
                });
            } else if (renderTarget->DepthTarget()) {
                // TODO:
            }
        }
    });
    
    std::vector<VkAttachmentReference> colorReferences;
    std::for_each(m_ColorBuffers.begin(), m_ColorBuffers.end(), [&](IRenderTargetPtr colorTarget) {
        if (colorTarget) {
            VkAttachmentReference colorReference = {
                .attachment = (uint32_t)colorReferences.size(),
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };
            colorReferences.push_back(colorReference);
        }
    });
    
    VkAttachmentReference depthReference = {
        .attachment = (uint32_t)m_ColorBuffers.size(),
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    
    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .flags = 0,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = (uint32_t)colorReferences.size(),
        .pColorAttachments = colorReferences.data(),
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = (DepthAttachement() ? &depthReference : nullptr),
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };
    
    VkRenderPassCreateInfo renderpassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .attachmentCount = (uint32_t)attachementDescription.size(),
        .pAttachments = attachementDescription.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 0,
        .pDependencies = nullptr,
    };
    
    // TODO: Logical device
    VkResult result = vkCreateRenderPass(m_LogicalDevice, &renderpassInfo, nullptr, &m_RenderPass);
    if (result != VK_SUCCESS) {
        m_RenderPass = nullptr;
    }
    
    // Framebuffers
    std::for_each(imageViews.begin(), imageViews.end(), [&](const VkImageView& imageView) {
        const VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = NULL,
            .renderPass = m_RenderPass,
            .attachmentCount = (uint32_t)imageViews.size(),
            .pAttachments = imageViews.data(),
            .width = (uint32_t)Width(),
            .height = (uint32_t)Height(),
            .layers = 1,
        };
        
        VkFramebuffer framebuffer;
        result = vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &framebuffer);
        if (result == VK_SUCCESS) {
            m_Framebuffers.push_back(framebuffer);
        } else {
            m_Framebuffers.clear();
            return;
        }
    });
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */