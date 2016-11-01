//
//  CRenderTargetStencilOGLES1_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#include "CRenderTargetStencilOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGLES1_0::CRenderTargetStencilOGLES1_0()
{
}

CRenderTargetStencilOGLES1_0::~CRenderTargetStencilOGLES1_0()
{
}

GLenum CRenderTargetStencilOGLES1_0::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
#if GL_OES_stencil8
        case Stencil8:
            return GL_STENCIL_INDEX8_OES;
            break;
#endif
            
        default:
            // Unacceptible type of stencil buffer
            assert(false);
            break;
    }
    
#if GL_OES_stencil8
    return GL_STENCIL_INDEX8_OES;
#else
    return 0; // TODO: error
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
