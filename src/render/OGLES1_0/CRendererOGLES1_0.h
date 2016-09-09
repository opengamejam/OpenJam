//
//  CRendererOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CRENDEREROGLES1_0_H
#define CRENDEREROGLES1_0_H

#include "CRendererOGL1_3.h"

namespace jam
{
    
class CRendererOGLES1_0 : public IRenderer
{
public:
    CRendererOGLES1_0(IRenderViewPtr renderView);
    virtual ~CRendererOGLES1_0();
    
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
    
private:
    IRenderViewPtr m_RenderView;
};
    
}; // namespace jam

#endif /* defined(CRENDEREROGLES1_0_H) */

#endif /* defined(RENDER_OGLES1_0) */