//
//  CRenderTargetColorOGLES1_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#include "CRenderTargetColorOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGLES1_0::CRenderTargetColorOGLES1_0()
{
}

CRenderTargetColorOGLES1_0::~CRenderTargetColorOGLES1_0()
{
}

GLenum CRenderTargetColorOGLES1_0::ConvertInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
        case ColorRGB888:
            return GL_RGB8_OES;
            break;
        case ColorRGB565:
            return GL_RGB565_OES;
            break;
        case ColorRGBA8888:
            return GL_RGBA8_OES;
            break;
        case ColorRGBA4444:
            return GL_RGBA4_OES;
            break;
    }
    
    return GL_RGB565_OES;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
