//
//  CRenderTargetColorOGLES2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#include "CRenderTargetColorOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGLES2_0::CRenderTargetColorOGLES2_0()
{
}

CRenderTargetColorOGLES2_0::~CRenderTargetColorOGLES2_0()
{
}

GLenum CRenderTargetColorOGLES2_0::ConvertInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
        case ColorRGB565:
            return GL_RGB565;
            break;
#if GL_OES_rgb8_rgba8
        case ColorRGBA8888:
            return GL_RGBA8_OES;
            break;
#endif
        case ColorRGBA4444:
            return GL_RGBA4;
            break;
            
        default:
            // Unacceptible type of color buffer
            assert(false);
            break;
    }
    
    return GL_RGBA4;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES2_0) */
