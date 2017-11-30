//
//  CMeshVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#ifndef CMESHVULKAN_H
#define CMESHVULKAN_H

#include "CMeshOGLBase.h"

namespace jam {

class CMeshVulkan : public CMeshOGLBase
{
    JAM_OBJECT
public:
    CMeshVulkan();
    virtual ~CMeshVulkan();

    virtual void Bind() override;
    virtual void Unbind() override;
    virtual bool IsValid() const override;

private:
    uint32_t m_VAO;
};

}; // namespace jam

#endif /* CMESHVULKAN_H */

#endif /* defined(RENDER_VULKAN) */
