//
//  CRenderTargetStencilVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#ifndef CRENDERTARGETSTENCILVULKAN_H
#define CRENDERTARGETSTENCILVULKAN_H

#include "CRenderTargetStencilOGLBase.h"

namespace jam {

class CRenderTargetStencilVulkan : public CRenderTargetStencilOGLBase
{
    JAM_OBJECT
public:
    CRenderTargetStencilVulkan();
    virtual ~CRenderTargetStencilVulkan();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertToInternalFormat(InternalFormats internalFormat) override;
};

}; // namespace jam

#endif /* CRENDERTARGETSTENCILVULKAN_H */

#endif /* defined(RENDER_VULKAN) */
