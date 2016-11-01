//
//  CRenderTargetColorOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#include "CRenderTargetColorOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGL2_0::CRenderTargetColorOGL2_0()
{
}

CRenderTargetColorOGL2_0::~CRenderTargetColorOGL2_0()
{
}

GLenum CRenderTargetColorOGL2_0::ConvertInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat) {
    case ColorR8:
        return GL_R8I;
        break;
    case ColorR16:
        return GL_R16I;
        break;
    case ColorR32:
        return GL_R32I;
        break;
    case ColorRGB888:
        return GL_RGB8;
        break;
    case ColorRGB565:
        return GL_RGB565;
        break;
    case ColorRGBA8888:
        return GL_RGBA8;
        break;
    case ColorRGBA4444:
        return GL_RGBA4;
        break;
    case ColorRGB10_A2:
        return GL_RGB10_A2;
        break;

    default:
        // Unacceptible type of color buffer
        assert(false);
        break;
    }

    return GL_RGB8;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */
