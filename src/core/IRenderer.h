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

class IRenderer {
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
    
    template<class T, class = typename std::enable_if<std::is_base_of<IFrameBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewFrameBuffer(uint32_t width, uint32_t height)
    {
        IFrameBufferPtr frameBuffer = CreateFrameBuffer(width, height);
        if (frameBuffer->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(frameBuffer);
        }
        
        assert(false && "Trying to create framebuffer with incorrect type");
        return nullptr;
    }

    /*
     * Create color render buffer
     */
    virtual CRenderTargetColorPtr CreateColorRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetColor, T>::value, T>::type>
    std::shared_ptr<T> NewColorRenderTarget()
    {
        CRenderTargetColorPtr colorTarget = CreateColorRenderTarget();
        if (colorTarget->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(colorTarget);
        }
        
        assert(false && "Trying to create color target with incorrect type");
        return nullptr;
    }

    /*
     * Create depth render buffer
     */
    virtual CRenderTargetDepthPtr CreateDepthRenderTarget() = 0;

    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetDepth, T>::value, T>::type>
    std::shared_ptr<T> NewDepthRenderTarget()
    {
        CRenderTargetDepthPtr depthTarget = CreateDepthRenderTarget();
        if (depthTarget->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(depthTarget);
        }
        
        assert(false && "Trying to create depth target with incorrect type");
        return nullptr;
    }
    
    /*
     * Create stencil render buffer
     */
    virtual CRenderTargetStencilPtr CreateStencilRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetStencil, T>::value, T>::type>
    std::shared_ptr<T> NewStencilRenderTarget()
    {
        CRenderTargetStencilPtr stencilTarget = CreateStencilRenderTarget();
        if (stencilTarget->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(stencilTarget);
        }
        
        assert(false && "Trying to create stencil target with incorrect type");
        return nullptr;
    }

    /*
     * Create render texture
     */
    virtual CRenderTargetTexturePtr CreateTextureRenderTarget() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<CRenderTargetTexture, T>::value, T>::type>
    std::shared_ptr<T> NewTextureRenderTarget()
    {
        CRenderTargetTexturePtr textureTarget = CreateTextureRenderTarget();
        if (textureTarget->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(textureTarget);
        }
        
        assert(false && "Trying to create texture target with incorrect type");
        return nullptr;
    }

    /*
     * Create vertex buffer object
     */
    virtual IVertexBufferPtr CreateVertexBuffer() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IVertexBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewVertexBuffer()
    {
        IVertexBufferPtr vertexBuffer = CreateVertexBuffer();
        if (vertexBuffer->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(vertexBuffer);
        }
        
        assert(false && "Trying to create vertex buffer with incorrect type");
        return nullptr;
    }

    /*
     * Create index buffer object
     */
    virtual IIndexBufferPtr CreateIndexBuffer() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IIndexBuffer, T>::value, T>::type>
    std::shared_ptr<T> NewIndexBuffer()
    {
        IIndexBufferPtr indexBuffer = CreateIndexBuffer();
        if (indexBuffer->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(indexBuffer);
        }
        
        assert(false && "Trying to create index buffer with incorrect type");
        return nullptr;
    }

    /*
     * Create texture
     */
    virtual ITexturePtr CreateTexture() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<ITexture, T>::value, T>::type>
    std::shared_ptr<T> NewTexture()
    {
        ITexturePtr texture = CreateTexture();
        if (texture->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(texture);
        }
        
        assert(false && "Trying to create texture with incorrect type");
        return nullptr;
    }

    /*
     * Create material
     */
    virtual IMaterialPtr CreateMaterial() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IMaterial, T>::value, T>::type>
    std::shared_ptr<T> NewMaterial()
    {
        IMaterialPtr material = CreateMaterial();
        if (material->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(material);
        }
        
        assert(false && "Trying to create material with incorrect type");
        return nullptr;
    }

    /*
     * Create mesh
     */
    virtual IMeshPtr CreateMesh() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IMesh, T>::value, T>::type>
    std::shared_ptr<T> NewMesh()
    {
        IMeshPtr mesh = CreateMesh();
        if (mesh->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(mesh);
        }
        
        assert(false && "Trying to create mesh with incorrect type");
        return nullptr;
    }

    /*
     * Create shader
     */
    virtual IShaderPtr CreateShader() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IShader, T>::value, T>::type>
    std::shared_ptr<T> NewShader()
    {
        IShaderPtr shader = CreateShader();
        if (shader->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(shader);
        }
        
        assert(false && "Trying to create shader with incorrect type");
        return nullptr;
    }

    /*
     * Create shader programe
     */
    virtual IShaderProgramPtr CreateShaderProgram() = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IShaderProgram, T>::value, T>::type>
    std::shared_ptr<T> NewShaderProgram()
    {
        IShaderProgramPtr shaderProgram = CreateShaderProgram();
        if (shaderProgram->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(shaderProgram);
        }
        
        assert(false && "Trying to create shader program with incorrect type");
        return nullptr;
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
