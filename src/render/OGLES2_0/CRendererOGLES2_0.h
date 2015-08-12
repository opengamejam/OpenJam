//
//  CRendererOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CRENDEREROGLES2_0_H
#define CRENDEREROGLES2_0_H

#include "IRenderer.h"

namespace jam
{
    
class CRendererOGLES2_0 : public IRenderer
{
public:
    CRendererOGLES2_0(IRenderViewPtr renderView);
    virtual ~CRendererOGLES2_0();
    
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

#endif /* defined(CRENDEREROGLES2_0_H) */

#endif // RENDER_OGLES2_0