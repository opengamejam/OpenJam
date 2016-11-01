//
//  CRenderTargetDepthOGLES1_1.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#include "CRenderTargetDepthOGLES1_1.h"
#include "CRenderTargetStencilOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGLES1_1::CRenderTargetDepthOGLES1_1()
{
}

CRenderTargetDepthOGLES1_1::~CRenderTargetDepthOGLES1_1()
{
}

GLenum CRenderTargetDepthOGLES1_1::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case Depth16:
        return GL_DEPTH_COMPONENT16_OES;
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

    return GL_DEPTH_COMPONENT24_OES;
}

CRenderTargetStencilPtr CRenderTargetDepthOGLES1_1::CreateStencilObject()
{
#if GL_EXT_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGLES1_1());
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

#endif /* defined(RENDER_OGLES1_1) */
