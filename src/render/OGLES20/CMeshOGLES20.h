//
//  CMeshOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#ifndef CMESHOGLES20_H
#define CMESHOGLES20_H

#include "IMesh.h"

namespace jam
{

class CMeshOGLES20 : public IMesh
{
public:
    CMeshOGLES20();
    virtual ~CMeshOGLES20();
    
    virtual void Bind();
    virtual void Unbind();
    virtual bool IsValid() const;
    
    virtual IVertexBufferPtr VertexBuffer() const;
    virtual void VertexBuffer(IVertexBufferPtr vertexBuffer);
    
    virtual IIndexBufferPtr IndexBuffer() const;
    virtual void IndexBuffer(IIndexBufferPtr indexBuffer);
    
private:
    unsigned int m_VAO;
    IVertexBufferPtr m_VertexBuffer;
    IIndexBufferPtr m_IndexBuffer;
};

}; // namespace jam

#endif /* defined(CMESHOGLES20_H) */

#endif // RENDER_OGLES2