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
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual bool IsValid() const override;
    
    virtual IVertexBufferPtr VertexBuffer() const override;
    virtual void VertexBuffer(IVertexBufferPtr vertexBuffer) override;
    
    virtual IIndexBufferPtr IndexBuffer() const override;
    virtual void IndexBuffer(IIndexBufferPtr indexBuffer) override;
    
private:
    IVertexBufferPtr m_VertexBuffer;
    IIndexBufferPtr m_IndexBuffer;
};

}; // namespace jam

#endif /* defined(CMeshOGL1_5_H) */

#endif /* CMESHOGL1_5_H || RENDER_OGLES1_1 */