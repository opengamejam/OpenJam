//
//  CVertexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#ifndef CVERTEXBUFFERVULKAN_H
#define CVERTEXBUFFERVULKAN_H

#include "CVertexBufferOGLBase.h"

namespace jam {

class CVertexBufferVulkan : public CVertexBufferOGLBase
{
    JAM_OBJECT
public:
    CVertexBufferVulkan();
    virtual ~CVertexBufferVulkan();

    virtual void Bind() override;
    virtual void Unbind() override;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFERVULKAN_H) */

#endif /* defined(RENDER_VULKAN) */
