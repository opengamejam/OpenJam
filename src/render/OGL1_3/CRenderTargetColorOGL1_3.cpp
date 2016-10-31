//
//  CRenderTargetColorOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#include "CRenderTargetColorOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetColorOGL1_3::CRenderTargetColorOGL1_3()
{
}

CRenderTargetColorOGL1_3::~CRenderTargetColorOGL1_3()
{
}

GLenum CRenderTargetColorOGL1_3::ConvertInternalFormat(InternalFormats internalFormat)
{
    switch (internalFormat)
    {
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
        case Depth16:
            return GL_DEPTH_COMPONENT16;
            break;
        case Depth24:
            return GL_DEPTH_COMPONENT24;
            break;
        case Depth32:
            return GL_DEPTH_COMPONENT32F;
            break;
        case Stencil8:
            return GL_STENCIL_INDEX8;
            break;
        case Depth24_Stencil8:
            return GL_DEPTH24_STENCIL8;
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

#endif /* defined(RENDER_OGL1_3) */
