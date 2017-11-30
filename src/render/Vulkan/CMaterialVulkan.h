//
//  CMaterialVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#ifndef CMATERIALVULKAN_H
#define CMATERIALVULKAN_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialVulkan : public CMaterialOGLBase
{
    JAM_OBJECT
public:
    CMaterialVulkan();
    virtual ~CMaterialVulkan();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALVULKAN_H */

#endif /* defined(RENDER_VULKAN) */
