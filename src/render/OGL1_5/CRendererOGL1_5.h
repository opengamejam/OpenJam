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
    
    virtual IRenderViewPtr RenderView() const;
    
    virtual IVertexBufferPtr CreatVertexBuffer();
    virtual IIndexBufferPtr CreateIndexBuffer();
    virtual ITexturePtr CreateTexture();
    virtual IMaterialPtr CreateMaterial();
    virtual IMeshPtr CreateMesh();
    virtual IShaderPtr CreateShader();
    virtual IShaderProgramPtr CreateShaderProgram();
    virtual IRenderTargetPtr CreateRenderTarget(unsigned int width, unsigned int height);
    
    virtual void Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader);
    virtual void Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material);
    virtual void Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material);
    
private:
    IRenderViewPtr m_RenderView;
};
    
}; // namespace jam

#endif /* defined(CRENDEREROGL1_5_H) */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */