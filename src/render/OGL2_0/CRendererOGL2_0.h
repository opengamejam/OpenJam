//
//  CRendererOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CRENDEREROGL2_0_H
#define CRENDEREROGL2_0_H

#include "IRenderer.h"

namespace jam
{
    
class CRendererOGL2_0 : public IRenderer
{
public:
    CRendererOGL2_0(IRenderViewPtr renderView);
    virtual ~CRendererOGL2_0();
    
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

#endif /* defined(CRENDEREROGL2_0_H) */

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0