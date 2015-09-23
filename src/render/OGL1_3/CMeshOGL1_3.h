//
//  CMeshOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CMESHOGL1_3_H
#define CMESHOGL1_3_H

#include "IMesh.h"

namespace jam
{

class CMeshOGL1_3 : public IMesh
{
public:
    CMeshOGL1_3();
    virtual ~CMeshOGL1_3();
    
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

#endif /* defined(CMeshOGL1_3_H) */

#endif /* RENDER_OGL1_3 */