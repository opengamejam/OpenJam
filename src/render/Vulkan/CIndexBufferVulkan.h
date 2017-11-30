//
//  CIndexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#ifndef CINDEXBUFFERVULKAN_H
#define CINDEXBUFFERVULKAN_H

#include "CIndexBufferOGLBase.h"

namespace jam {

class CIndexBufferVulkan : public CIndexBufferOGLBase
{
    JAM_OBJECT
public:
    CIndexBufferVulkan();
    virtual ~CIndexBufferVulkan();
};

}; // namespace jam

#endif /* CINDEXBUFFERVULKAN_H */

#endif /* defined(RENDER_VULKAN) */
