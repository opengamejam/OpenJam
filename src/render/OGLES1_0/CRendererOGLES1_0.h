//
//  CRendererOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CRENDEREROGLES1_0_H
#define CRENDEREROGLES1_0_H

#include "CRendererOGL1_3.h"

namespace jam
{
    
class CRendererOGLES1_0 : public IRenderer
{
public:
    CRendererOGLES1_0(IRenderViewPtr renderView);
    virtual ~CRendererOGLES1_0();
    
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

#endif /* defined(CRENDEREROGLES1_0_H) */

#endif /* RENDER_OGL1_3 */