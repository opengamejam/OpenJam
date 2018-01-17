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
#include "CRendererVulkan.h"
#include "IRenderView.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferVulkan::CFrameBufferVulkan(uint32_t width, uint32_t height, CRendererVulkanPtr renderer)
: m_IsInitialized(false)
, m_Width(width)
, m_Height(height)
, m_Renderer(renderer)
, m_NumColorAtachments(1)
, m_RenderPass(nullptr)
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
       //DepthAttachement()->Ptr<CFrameBufferDepthVulkan>()->Allocate(width, height);
    }
    
    if (StencilAttachement()) {
        //StencilAttachement()->Ptr<CRenderTargetStencilVulkan>()->Allocate(width, height);
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
        m_ColorBuffers.resize(index + 1);
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

const std::vector<VkFramebuffer>& CFrameBufferVulkan::FrameBuffers() const
{
    return m_Framebuffers;
}

const VkRenderPass& CFrameBufferVulkan::RenderPass() const
{
    return m_RenderPass;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CFrameBufferVulkan::Rebuild()
{
    CRendererVulkanPtr renderer = m_Renderer.lock();
    IRenderViewPtr renderView = renderer->RenderView();
    CRenderInstanceVulkanPtr renderInstance = renderView->RenderInstance()->Ptr<CRenderInstanceVulkan>();
    if (!renderInstance) {
        JAM_LOG("CFrameBufferVulkan::Rebuild - Cannot rebuild framebuffer without render instance");
        return;
    }
    
    if (m_RenderPass) {
        vkDestroyRenderPass(renderer->LogicalDevice(), m_RenderPass, nullptr);
    }
    std::for_each(m_Framebuffers.begin(), m_Framebuffers.end(), [&](const VkFramebuffer& framebuffer) {
        vkDestroyFramebuffer(renderer->LogicalDevice(), framebuffer, nullptr);
    });
    m_Framebuffers.clear();
    
    std::vector<IRenderTargetPtr> renderTargets(m_ColorBuffers);
    renderTargets.push_back(m_DepthBuffer);
    
    std::vector<VkImageView> colorImageViews;
    std::vector<VkImageView> depthImageViews;
    std::vector<VkAttachmentDescription> attachementDescription;
    std::for_each(renderTargets.begin(), renderTargets.end(), [&](IRenderTargetPtr renderTarget) {
        if (renderTarget) {
            VkFormat format = VK_FORMAT_UNDEFINED;
            VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
            if (renderTarget->ColorTarget()) {
                format = renderTarget->Ptr<CRenderTargetColorVulkan>()->Format();
                layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                
                const std::vector<VkImageView>& views = renderTarget->Ptr<CRenderTargetColorVulkan>()->ImageViews();
                std::copy (views.begin(), views.end(), std::back_inserter(colorImageViews));
            } else if (renderTarget->DepthTarget()) {
                format = renderTarget->Ptr<CRenderTargetDepthVulkan>()->Format();
                layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                
                const std::vector<VkImageView>& views = renderTarget->Ptr<CRenderTargetDepthVulkan>()->ImageViews();
                std::copy (views.begin(), views.end(), std::back_inserter(depthImageViews));
            }/* else if (renderTarget->StencilTarget()) {
                format = renderTarget->Ptr<CRenderTargetStencilVulkan>()->Format();
            } else if (renderTarget->TextureTarget()) {
                format = renderTarget->Ptr<CRenderTargetTextureVulkan>()->Format();
            }*/
            assert(format != VK_FORMAT_UNDEFINED);
            
            VkAttachmentDescription desc = {
                .flags = 0,
                .format = format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = layout,
            };
            attachementDescription.push_back(desc);
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
        .attachment = (uint32_t)colorReferences.size(),
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
    VkResult result = vkCreateRenderPass(renderer->LogicalDevice(), &renderpassInfo, nullptr, &m_RenderPass);
    if (result != VK_SUCCESS) {
        m_RenderPass = nullptr;
    }
    
    // Framebuffers
    std::for_each(colorImageViews.begin(), colorImageViews.end(), [&](const VkImageView& imageView) {
        std::vector<VkImageView> attachments = {imageView};
        if (depthImageViews.size() > 0) {
            attachments.push_back(depthImageViews[0]);
        }
        
        const VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = NULL,
            .renderPass = m_RenderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = (uint32_t)Width(),
            .height = (uint32_t)Height(),
            .layers = 1,
        };
        
        VkFramebuffer framebuffer;
        result = vkCreateFramebuffer(renderer->LogicalDevice(), &framebufferInfo, nullptr, &framebuffer);
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
