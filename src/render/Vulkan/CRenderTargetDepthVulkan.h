 
    
//
//  CRenderTargetDepthVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#ifndef CRENDERTARGETDEPTHVULKAN_H
#define CRENDERTARGETDEPTHVULKAN_H

#include "CRenderTargetDepthOGLBase.h"

namespace jam {

class CRenderTargetDepthVulkan : public CRenderTargetDepthOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetDepthVulkan();
    virtual ~CRenderTargetDepthVulkan();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
    virtual CRenderTargetStencilPtr CreateStencilObject() override;
};

}; // namespace jam

#endif /* CRENDERTARGETDEPTHVULKAN_H */

#endif /* defined(RENDER_VULKAN) */

