//
//  CMeshVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CMESHVULKAN_H
#define CMESHVULKAN_H

#include "IMesh.h"

namespace jam {

class CMeshVulkan : public IMesh
{
    JAM_OBJECT
public:
    CMeshVulkan();
    virtual ~CMeshVulkan();

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

#endif /* CMESHVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
