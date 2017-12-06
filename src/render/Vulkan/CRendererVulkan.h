//
//  CRendererVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_Vulkan)

#ifndef CRENDERERVULKAN_H
#define CRENDERERVULKAN_H

#include "IRenderer.h"

namespace jam {
    
CLASS_PTR(CRendererVulkan)

class CRendererVulkan : public IRenderer
{
    JAM_OBJECT
public:
    CRendererVulkan(IRenderViewPtr renderView);
    virtual ~CRendererVulkan();

    /*
     *
     */
    virtual void Initialize();
    
    /*
     * Returns render view
     */
    virtual IRenderViewPtr RenderView() const override;
    
    /*
     * Create framebuffer object
     */
    virtual IFrameBufferPtr CreateFrameBuffer(uint32_t width, uint32_t height) override;

    /*
     * Create color render buffer
     */
    virtual CRenderTargetColorPtr CreateColorRenderTarget() override;

    /*
     * Create depth render buffer
     */
    virtual CRenderTargetDepthPtr CreateDepthRenderTarget() override;

    /*
     * Create stencil render buffer
     */
    virtual CRenderTargetStencilPtr CreateStencilRenderTarget() override;

    /*
     * Create render texture
     */
    virtual CRenderTargetTexturePtr CreateTextureRenderTarget() override;

    /*
     * Create vertex buffer object
     */
    virtual IVertexBufferPtr CreateVertexBuffer() override;

    /*
     * Create index buffer object
     */
    virtual IIndexBufferPtr CreateIndexBuffer() override;

    /*
     * Create texture
     */
    virtual ITexturePtr CreateTexture() override;

    /*
     * Create material
     */
    virtual IMaterialPtr CreateMaterial() override;

    /*
     * Create mesh
     */
    virtual IMeshPtr CreateMesh() override;

    /*
     * Create shader
     */
    virtual IShaderPtr CreateShader() override;

    /*
     * Create shader programe
     */
    virtual IShaderProgramPtr CreateShaderProgram() override;

    /*
     * Draw mesh with material properties and shader
     */
    virtual void Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader) override;

    /*
     * Draw triangles with material properties
     */
    virtual void Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material) override;

    /*
     * Draw indexed triangles with material properties
     */
    virtual void Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material) override;
    
    /*
     * Vulkan specific
     */
    const VkDevice& LogicalDevice() const;
    const VkQueue& Queue() const;
    const std::vector<VkCommandBuffer>& CommandBuffers() const;
    
private:
    /*
     *
     */
    std::vector<VkQueueFamilyProperties> GetPhysicalDeviceQueueProps(const VkInstance& instance,
                                                                     const VkPhysicalDevice physicalDevice);
    std::tuple<VkResult, VkDevice> CreateLogicalDevice(const VkPhysicalDevice& physicalDevice /* debug layer */);
    std::tuple<bool, VkQueueFamilyProperties, uint32_t> FindQueueProperties(VkQueueFlags flag,
                                                                            const std::vector<VkBool32>& supportsPresent);
    
private:
    IRenderViewPtr m_RenderView;
    std::vector<VkQueueFamilyProperties> m_QueueProperties;
    VkDevice m_LogicalDevice;
    VkCommandPool m_CommandPool;
    std::vector<VkCommandBuffer> m_CommandBuffers;
    VkQueue m_Queue;
};

}; // namespace jam

#endif /* defined(CRENDERERVULKAN_H) */

//#endif /* defined(RENDER_VULKAN) */
