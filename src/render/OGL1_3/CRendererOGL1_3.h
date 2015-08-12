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

#endif /* defined(CRENDEREROGL1_3_H) */

#endif /* RENDER_OGL1_3 */