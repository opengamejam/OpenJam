//
//  CRendererOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CRENDEREROGLES2_0_H
#define CRENDEREROGLES2_0_H

#include "CRendererOGLBase.h"

namespace jam
{
    
class CRendererOGLES2_0 : public CRendererOGLBase
{
public:
    CRendererOGLES2_0(IRenderViewPtr renderView);
    virtual ~CRendererOGLES2_0();
    
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
};
    
}; // namespace jam

#endif /* defined(CRENDEREROGLES2_0_H) */

#endif /* defined(RENDER_OGLES2_0) */
