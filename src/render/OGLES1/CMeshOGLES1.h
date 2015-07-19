//
//  CMeshOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#ifndef CMESHOGLES1_H
#define CMESHOGLES1_H

#include "IMesh.h"

namespace jam
{

class CMeshOGLES1 : public IMesh
{
public:
    CMeshOGLES1();
    virtual ~CMeshOGLES1();
    
    virtual void Bind();
    virtual void Unbind();
    virtual bool IsValid() const;
    
    virtual IVertexBufferPtr VertexBuffer() const;
    virtual void VertexBuffer(IVertexBufferPtr vertexBuffer);
    
    virtual IIndexBufferPtr IndexBuffer() const;
    virtual void IndexBuffer(IIndexBufferPtr indexBuffer);
    
private:
    IVertexBufferPtr m_VertexBuffer;
    IIndexBufferPtr m_IndexBuffer;
};

}; // namespace jam

#endif /* defined(CMeshOGLES1_H) */

#endif /* CMESHOGLES1_H */