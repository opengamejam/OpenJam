//
//  CRendererOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CRENDEREROGLES1_1_H
#define CRENDEREROGLES1_1_H

#include "IRenderer.h"

namespace jam
{
    
class CRendererOGLES1_1 : public IRenderer
{
public:
    CRendererOGLES1_1(IRenderViewPtr renderView);
    virtual ~CRendererOGLES1_1();
    
    virtual IRenderViewPtr RenderView() const override;
    
    virtual IVertexBufferPtr CreatVertexBuffer() override;
    virtual IIndexBufferPtr CreateIndexBuffer() override;
    virtual ITexturePtr CreateTexture() override;
    virtual IMaterialPtr CreateMaterial() override;
    virtual IMeshPtr CreateMesh() override;
    virtual IShaderPtr CreateShader() override;
    virtual IShaderProgramPtr CreateShaderProgram() override;
    virtual IRenderTargetPtr CreateRenderTarget(uint32_t width, uint32_t height) override;
    
    virtual void Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader) override;
    virtual void Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material) override;
    virtual void Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material) override;
    
private:
    IRenderViewPtr m_RenderView;
};
    
}; // namespace jam

#endif /* defined(CRENDEREROGLES1_1_H) */

#endif /* RENDER_OGLES1_1 */