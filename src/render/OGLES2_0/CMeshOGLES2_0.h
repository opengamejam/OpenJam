//
//  CMeshOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CMESHOGLES2_0_H
#define CMESHOGLES2_0_H

#include "IMesh.h"

namespace jam
{

class CMeshOGLES2_0 : public IMesh
{
public:
    CMeshOGLES2_0();
    virtual ~CMeshOGLES2_0();
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual bool IsValid() const override;
    
    virtual IVertexBufferPtr VertexBuffer() const override;
    virtual void VertexBuffer(IVertexBufferPtr vertexBuffer) override;
    
    virtual IIndexBufferPtr IndexBuffer() const override;
    virtual void IndexBuffer(IIndexBufferPtr indexBuffer) override;
    
private:
    uint32_t m_VAO;
    IVertexBufferPtr m_VertexBuffer;
    IIndexBufferPtr m_IndexBuffer;
};

}; // namespace jam

#endif /* defined(CMESHOGLES2_0_H) */

#endif // RENDER_OGLES2_0