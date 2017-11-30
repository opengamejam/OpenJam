//
//  CRenderTargetStencilVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#include "CRenderTargetStencilVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilVulkan::CRenderTargetStencilVulkan()
{
}

CRenderTargetStencilVulkan::~CRenderTargetStencilVulkan()
{
}

GLenum CRenderTargetStencilVulkan::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case Stencil8:
        return GL_STENCIL_INDEX8_OES;
        break;

    default:
        // Unacceptible type of stencil buffer
        assert(false);
        break;
    }

    return GL_STENCIL_INDEX8_OES;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_VULKAN) */
