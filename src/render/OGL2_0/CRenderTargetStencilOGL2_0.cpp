//
//  CRenderTargetStencilOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#include "CRenderTargetStencilOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetStencilOGL2_0::CRenderTargetStencilOGL2_0()
{
}

CRenderTargetStencilOGL2_0::~CRenderTargetStencilOGL2_0()
{
}

GLenum CRenderTargetStencilOGL2_0::ConvertToInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
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

#endif /* defined(RENDER_OGL2_0) */
