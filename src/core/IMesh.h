//
//  IMesh.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IMESH_H
#define IMESH_H

#include "Global.h"

namespace jam
{

CLASS_PTR(IVertexBuffer)
CLASS_PTR(IIndexBuffer)
    
class IMesh
{
    JAM_OBJECT_BASE
public:
    IMesh() = default;
    virtual ~IMesh() = default;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual bool IsValid() const = 0;
    
    virtual IVertexBufferPtr VertexBuffer() const = 0;
    virtual void VertexBuffer(IVertexBufferPtr vertexBuffer) = 0;
    
    virtual IIndexBufferPtr IndexBuffer() const = 0;
    virtual void IndexBuffer(IIndexBufferPtr indexBuffer) = 0;
};

}; // namespace jam

#endif /* IMESH_H */
