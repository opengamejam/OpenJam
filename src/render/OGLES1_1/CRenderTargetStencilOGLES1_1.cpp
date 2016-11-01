//
//  CRenderTargetStencilOGLES1_1.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#include "CRenderTargetStencilOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGLES1_1::CRenderTargetStencilOGLES1_1()
{
}

CRenderTargetStencilOGLES1_1::~CRenderTargetStencilOGLES1_1()
{
}

GLenum CRenderTargetStencilOGLES1_1::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case Stencil8:
        return GL_STENCIL_INDEX8;
        break;

    default:
        // Unacceptible type of stencil buffer
        assert(false);
        break;
    }

    return GL_STENCIL_INDEX8;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_1) */
