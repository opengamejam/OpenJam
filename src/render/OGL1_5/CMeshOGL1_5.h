//
//  CMeshOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CMESHOGL1_5_H
#define CMESHOGL1_5_H

#include "IMesh.h"

namespace jam
{

class CMeshOGL1_5 : public IMesh
{
public:
    CMeshOGL1_5();
    virtual ~CMeshOGL1_5();
    
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

#endif /* defined(CMeshOGL1_5_H) */

#endif /* CMESHOGL1_5_H || RENDER_OGLES1_1 */