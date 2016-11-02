//
//  CRenderTargetTextureOGLES2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES2_0)

#include "CRenderTargetTextureOGLES2_0.h"
#include "CTextureOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGLES2_0::CRenderTargetTextureOGLES2_0()
{
}

CRenderTargetTextureOGLES2_0::~CRenderTargetTextureOGLES2_0()
{
}

ITexturePtr CRenderTargetTextureOGLES2_0::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGLES2_0());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES2_0) */
