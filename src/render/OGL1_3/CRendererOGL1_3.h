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

#endif /* defined(CRENDEREROGL1_3_H) */

#endif /* RENDER_OGL1_3 */