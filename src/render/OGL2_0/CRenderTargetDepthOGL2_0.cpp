//
//  CRenderTargetDepthOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#include "CRenderTargetDepthOGL2_0.h"
#include "CRenderTargetStencilOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL2_0::CRenderTargetDepthOGL2_0()
{
}

CRenderTargetDepthOGL2_0::~CRenderTargetDepthOGL2_0()
{
}

GLenum CRenderTargetDepthOGL2_0::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case Depth16:
        return GL_DEPTH_COMPONENT16;
        break;
    case Depth24:
        return GL_DEPTH_COMPONENT24;
        break;
    case Depth32:
        return GL_DEPTH_COMPONENT32F;
        break;
#if GL_EXT_packed_depth_stencil
    case Depth24_Stencil8:
        return GL_DEPTH24_STENCIL8;
        break;
#endif

    default:
        // Unacceptible type of depth buffer
        assert(false);
        break;
    }

    return GL_DEPTH_COMPONENT24;
}

CRenderTargetStencilPtr CRenderTargetDepthOGL2_0::CreateStencilObject()
{
#if GL_EXT_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGL2_0());
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

#endif /* defined(RENDER_OGL2_0) */


