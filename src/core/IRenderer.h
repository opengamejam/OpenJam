//
//  IRenderer.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IRENDERER_H
#define IRENDERER_H

#include "Global.h"

namespace jam
{
CLASS_PTR(IRenderer)
CLASS_PTR(IRenderView)
CLASS_PTR(IFrameBuffer)
CLASS_PTR(CRenderTargetColor)
CLASS_PTR(CRenderTargetDepth)
CLASS_PTR(CRenderTargetStencil)
CLASS_PTR(CRenderTargetTexture)
CLASS_PTR(IVertexBuffer)
CLASS_PTR(IIndexBuffer)
CLASS_PTR(ITexture)
CLASS_PTR(IMaterial)
CLASS_PTR(IMesh)
CLASS_PTR(IShader)
CLASS_PTR(IShaderProgram)
    
class IRenderer
{
    JAM_OBJECT_BASE
public:
    IRenderer() = default;
    virtual ~IRenderer() = default;
    
    /*
     * Returns render view
     */
    virtual IRenderViewPtr RenderView() const = 0;
    
    /*
     * Create framebuffer object
     */
    virtual IFrameBufferPtr CreateFrameBuffer(uint32_t width, uint32_t height) = 0;
    
    /*
     * Create color render buffer
     */
    virtual CRenderTargetColorPtr CreateColorRenderTarget() = 0;
    
    /*
     * Create depth render buffer
     */
    virtual CRenderTargetDepthPtr CreateDepthRenderTarget() = 0;
    
    /*
     * Create stencil render buffer
     */
    virtual CRenderTargetStencilPtr CreateStencilRenderTarget() = 0;
    
    /*
     * Create render texture
     */
    virtual CRenderTargetTexturePtr CreateTextureRenderTarget() = 0;
    
    /*
     * Create vertex buffer object
     */
    virtual IVertexBufferPtr CreateVertexBuffer() = 0;
    
    /*
     * Create index buffer object
     */
    virtual IIndexBufferPtr CreateIndexBuffer() = 0;
    
    /*
     * Create texture
     */
    virtual ITexturePtr CreateTexture() = 0;
    
    /*
     * Create material
     */
    virtual IMaterialPtr CreateMaterial() = 0;
    
    /*
     * Create mesh
     */
    virtual IMeshPtr CreateMesh() = 0;
    
    /*
     * Create shader
     */
    virtual IShaderPtr CreateShader() = 0;
    
    /*
     * Create shader programe
     */
    virtual IShaderProgramPtr CreateShaderProgram() = 0;
    
    /*
     * Draw mesh with material properties and shader
     */
    virtual void Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader) = 0;
    
    /*
     * Draw triangles with material properties
     */
    virtual void Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material) = 0;
    
    /*
     * Draw indexed triangles with material properties
     */
    virtual void Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material) = 0;
};
    
}; // namespace jam
    
#endif /* IRENDERER_H */
