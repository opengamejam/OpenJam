//
//  CRenderTargetDepthOGLES1_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#include "CRenderTargetDepthOGLES1_0.h"
#include "CRenderTargetStencilOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGLES1_0::CRenderTargetDepthOGLES1_0()
{
}

CRenderTargetDepthOGLES1_0::~CRenderTargetDepthOGLES1_0()
{
}

GLenum CRenderTargetDepthOGLES1_0::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
        case Depth16:
            return GL_DEPTH_COMPONENT16_OES;
            break;
        case Depth24:
            return GL_DEPTH_COMPONENT24_OES;
            break;
#if GL_EXT_packed_depth_stencil
        case Depth24_Stencil8:
            return GL_DEPTH24_STENCIL8_OES;
            break;
#endif
            
        default:
            // Unacceptible type of depth buffer
            assert(false);
            break;
    }
    
    return GL_DEPTH_COMPONENT16_OES;
}

CRenderTargetStencilPtr CRenderTargetDepthOGLES1_0::CreateStencilObject()
{
#if GL_EXT_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGLES1_0());
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

#endif /* defined(RENDER_OGLES1_0) */
