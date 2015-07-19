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
CLASS_PTR(IRenderer);
CLASS_PTR(IRenderView);
CLASS_PTR(IVertexBuffer);
CLASS_PTR(IIndexBuffer);
CLASS_PTR(ITexture);
CLASS_PTR(IMaterial);
CLASS_PTR(IMesh);
CLASS_PTR(IShader);
CLASS_PTR(IShaderProgram);
CLASS_PTR(IRenderTarget);
    
class IRenderer
{
public:
    IRenderer() = default;
    virtual ~IRenderer() = default;
    
    virtual IRenderViewPtr RenderView() const = 0;
    
    virtual IVertexBufferPtr CreatVertexBuffer() = 0;
    virtual IIndexBufferPtr CreateIndexBuffer() = 0;
    virtual ITexturePtr CreateTexture() = 0;
    virtual IMaterialPtr CreateMaterial() = 0;
    virtual IMeshPtr CreateMesh() = 0;
    virtual IShaderPtr CreateShader() = 0;
    virtual IShaderProgramPtr CreateShaderProgram() = 0;
    virtual IRenderTargetPtr CreateRenderTarget(unsigned int width, unsigned int height) = 0;
    
    virtual void Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader) = 0;
    virtual void Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material) = 0;
    virtual void Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material) = 0;
};
    
}; // namespace jam
    
#endif /* IRENDERER_H */
