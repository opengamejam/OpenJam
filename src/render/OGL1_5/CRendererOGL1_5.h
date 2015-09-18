//
//  CRendererOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CRENDEREROGL1_5_H
#define CRENDEREROGL1_5_H

#include "IRenderer.h"

namespace jam
{
    
class CRendererOGL1_5 : public IRenderer
{
public:
    CRendererOGL1_5(IRenderViewPtr renderView);
    virtual ~CRendererOGL1_5();
    
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

#endif /* defined(CRENDEREROGL1_5_H) */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */