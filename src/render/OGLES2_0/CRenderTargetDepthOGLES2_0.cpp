//
//  CRenderTargetDepthOGLES2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#include "CRenderTargetDepthOGLES2_0.h"
#include "CRenderTargetStencilOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGLES2_0::CRenderTargetDepthOGLES2_0()
{
}

CRenderTargetDepthOGLES2_0::~CRenderTargetDepthOGLES2_0()
{
}

GLenum CRenderTargetDepthOGLES2_0::ConvertToInternalFormat(InternalFormats internalFormat)
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

CRenderTargetStencilPtr CRenderTargetDepthOGLES2_0::CreateStencilObject()
{
#if GL_OES_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGLES2_0());
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

#endif /* defined(RENDER_OGLES2_0) */
