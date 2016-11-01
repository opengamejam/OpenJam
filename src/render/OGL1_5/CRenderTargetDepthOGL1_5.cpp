//
//  CRenderTargetDepthOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#include "CRenderTargetDepthOGL1_5.h"
#include "CRenderTargetStencilOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL1_5::CRenderTargetDepthOGL1_5()
{
}

CRenderTargetDepthOGL1_5::~CRenderTargetDepthOGL1_5()
{
}

GLenum CRenderTargetDepthOGL1_5::ConvertToInternalFormat(InternalFormats internalFormat)
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
    case Depth24_Stencil8:
        return GL_DEPTH24_STENCIL8;
        break;

    default:
        // Unacceptible type of depth buffer
        assert(false);
        break;
    }

    return GL_DEPTH_COMPONENT24;
}

CRenderTargetStencilPtr CRenderTargetDepthOGL1_5::CreateStencilObject()
{
#if GL_EXT_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGL1_5());
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

#endif /* defined(RENDER_OGL1_5) */
