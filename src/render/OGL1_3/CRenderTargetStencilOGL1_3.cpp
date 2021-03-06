//
//  CRenderTargetStencilOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#include "CRenderTargetStencilOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGL1_3::CRenderTargetStencilOGL1_3()
{
}

CRenderTargetStencilOGL1_3::~CRenderTargetStencilOGL1_3()
{
}

GLenum CRenderTargetStencilOGL1_3::ConvertToInternalFormat(InternalFormats internalFormat)
{
#if !defined(OS_KOS)
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
#else
    assert(false);
    return 0;
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) */
