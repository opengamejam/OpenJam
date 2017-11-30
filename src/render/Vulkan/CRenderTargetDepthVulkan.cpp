//
//  CRenderTargetDepthVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#include "CRenderTargetDepthVulkan.h"
#include "CRenderTargetStencilVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthVulkan::CRenderTargetDepthVulkan()
{
}

CRenderTargetDepthVulkan::~CRenderTargetDepthVulkan()
{
}

GLenum CRenderTargetDepthVulkan::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case Depth16:
        return GL_DEPTH_COMPONENT16;
        break;
#if GL_OES_depth24
    case Depth24:
        return GL_DEPTH_COMPONENT24_OES;
        break;
#endif
#if GL_EXT_texture_storage
    case Depth32:
        return GL_DEPTH_COMPONENT32_OES;
        break;
#endif
#if GL_OES_packed_depth_stencil
    case Depth24_Stencil8:
        return GL_DEPTH24_STENCIL8_OES;
        break;
#endif

    default:
        // Unacceptible type of depth buffer
        assert(false);
        break;
    }

    return GL_DEPTH_COMPONENT16;
}

CRenderTargetStencilPtr CRenderTargetDepthVulkan::CreateStencilObject()
{
#if GL_OES_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilVulkan());
    return stencil;
#else
    return nullptr;
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_VULKAN) */
