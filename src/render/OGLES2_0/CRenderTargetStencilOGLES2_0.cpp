//
//  CRenderTargetStencilOGLES2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#include "CRenderTargetStencilOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGLES2_0::CRenderTargetStencilOGLES2_0()
{
}

CRenderTargetStencilOGLES2_0::~CRenderTargetStencilOGLES2_0()
{
}

GLenum CRenderTargetStencilOGLES2_0::ConvertToInternalFormat(InternalFormats internalFormat)
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

#endif /* defined(RENDER_OGLES2_0) */
