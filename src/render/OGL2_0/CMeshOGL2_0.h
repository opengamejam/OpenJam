//
//  CMeshOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CMESHOGL2_0_H
#define CMESHOGL2_0_H

#include "IMesh.h"

namespace jam
{

class CMeshOGL2_0 : public IMesh
{
public:
    CMeshOGL2_0();
    virtual ~CMeshOGL2_0();
    
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

#endif /* defined(CMESHOGL2_0_H) */

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0