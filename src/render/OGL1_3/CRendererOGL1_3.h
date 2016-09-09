//
//  CRendererOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDEREROGL1_3_H
#define CRENDEREROGL1_3_H

#include "IRenderer.h"

namespace jam
{
    
class CRendererOGL1_3 : public IRenderer
{
public:
    CRendererOGL1_3(IRenderViewPtr renderView);
    virtual ~CRendererOGL1_3();
    
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

#endif /* defined(CRENDEREROGL1_3_H) */

#endif /* RENDER_OGL1_3 */