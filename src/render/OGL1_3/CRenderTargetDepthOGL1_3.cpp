//
//  CRenderTargetDepthOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#include "CRenderTargetDepthOGL1_3.h"
#include "CRenderTargetStencilOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetDepthOGL1_3::CRenderTargetDepthOGL1_3()
{
}

CRenderTargetDepthOGL1_3::~CRenderTargetDepthOGL1_3()
{
}

GLenum CRenderTargetDepthOGL1_3::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
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

CRenderTargetStencilPtr CRenderTargetDepthOGL1_3::CreateStencilObject()
{
#if GL_EXT_packed_depth_stencil
    CRenderTargetStencilPtr stencil(new CRenderTargetStencilOGL1_3());
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

#endif /* defined(RENDER_OGL1_3) */
