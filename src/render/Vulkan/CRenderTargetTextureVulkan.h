//
//  CRenderTargetTextureVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#ifndef CRENDERTARGETTEXTUREVULKAN_H
#define CRENDERTARGETTEXTUREVULKAN_H

#include "CRenderTargetTextureOGLBase.h"

namespace jam {

class CRenderTargetTextureVulkan : public CRenderTargetTextureOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetTextureVulkan();
    virtual ~CRenderTargetTextureVulkan();
    
    /*
     * OpenGL specific
     */
    virtual ITexturePtr CreateTextureObject() const override;
};

}; // namespace jam

#endif /* CRENDERTARGETTEXTUREVULKAN_H */

#endif /* defined(RENDER_VULKAN) */
