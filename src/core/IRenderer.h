//
//  IRenderer.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IRENDERER_H
#define IRENDERER_H

#include "IFrameBuffer.h"
#include "IRenderTarget.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IMesh.h"
#include "IShader.h"
#include "IShaderProgram.h"

namespace jam {
    
CLASS_PTR(IRenderer)
CLASS_PTR(IRenderView)

class IRenderer : public IObject<IRenderer> {
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
    
    template<class T, class = typename std::enable_if<std::is_base_of<IFrameBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewFrameBuffer(uint32_t width, uint32_t height)
    {
        IFrameBufferPtr frameBuffer = CreateFrameBuffer(width, height);
        return frameBuffer->Ptr<T>();
    }

    /*
     * Create color render buffer
     */
    virtual CRenderTargetColorPtr CreateColorRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetColor, T>::value, T>::type>
    std::shared_ptr<T> NewColorRenderTarget()
    {
        CRenderTargetColorPtr colorTarget = CreateColorRenderTarget();
        return colorTarget->Ptr<T>();
    }

    /*
     * Create depth render buffer
     */
    virtual CRenderTargetDepthPtr CreateDepthRenderTarget() = 0;

    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetDepth, T>::value, T>::type>
    std::shared_ptr<T> NewDepthRenderTarget()
    {
        CRenderTargetDepthPtr depthTarget = CreateDepthRenderTarget();
        return depthTarget->Ptr<T>();
    }
    
    /*
     * Create stencil render buffer
     */
    virtual CRenderTargetStencilPtr CreateStencilRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetStencil, T>::value, T>::type>
    std::shared_ptr<T> NewStencilRenderTarget()
    {
        CRenderTargetStencilPtr stencilTarget = CreateStencilRenderTarget();
        return stencilTarget->Ptr<T>();
    }

    /*
     * Create render texture
     */
    virtual CRenderTargetTexturePtr CreateTextureRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetTexture, T>::value, T>::type>
    std::shared_ptr<T> NewTextureRenderTarget()
    {
        CRenderTargetTexturePtr textureTarget = CreateTextureRenderTarget();
        return textureTarget->Ptr<T>();
    }

    /*
     * Create vertex buffer object
     */
    virtual IVertexBufferPtr CreateVertexBuffer() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IVertexBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewVertexBuffer()
    {
        IVertexBufferPtr vertexBuffer = CreateVertexBuffer();
        return vertexBuffer->Ptr<T>();
    }

    /*
     * Create index buffer object
     */
    virtual IIndexBufferPtr CreateIndexBuffer() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IIndexBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewIndexBuffer()
    {
        IIndexBufferPtr indexBuffer = CreateIndexBuffer();
        return indexBuffer->Ptr<T>();
    }

    /*
     * Create texture
     */
    virtual ITexturePtr CreateTexture() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<ITexture, T>::value, T>::type>
    std::shared_ptr<T> NewTexture()
    {
        ITexturePtr texture = CreateTexture();
        return texture->Ptr<T>();
    }

    /*
     * Create material
     */
    virtual IMaterialPtr CreateMaterial() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IMaterial, T>::value, T>::type>
    std::shared_ptr<T> NewMaterial()
    {
        IMaterialPtr material = CreateMaterial();
        return material->Ptr<T>();
    }

    /*
     * Create mesh
     */
    virtual IMeshPtr CreateMesh() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IMesh, T>::value, T>::type>
    std::shared_ptr<T> NewMesh()
    {
        IMeshPtr mesh = CreateMesh();
        return mesh->Ptr<T>();
    }

    /*
     * Create shader
     */
    virtual IShaderPtr CreateShader() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IShader, T>::value, T>::type>
    std::shared_ptr<T> NewShader()
    {
        IShaderPtr shader = CreateShader();
        return shader->Ptr<T>();
    }

    /*
     * Create shader programe
     */
    virtual IShaderProgramPtr CreateShaderProgram() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IShaderProgram, T>::value, T>::type>
    std::shared_ptr<T> NewShaderProgram()
    {
        IShaderProgramPtr shaderProgram = CreateShaderProgram();
        return shaderProgram->Ptr<T>();
    }

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
