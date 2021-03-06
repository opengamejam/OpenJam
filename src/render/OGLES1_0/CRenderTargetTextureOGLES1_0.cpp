//
//  CRenderTargetTextureOGLES1_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_0)

#include "CRenderTargetTextureOGLES1_0.h"
#include "CTextureOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGLES1_0::CRenderTargetTextureOGLES1_0()
{
}

CRenderTargetTextureOGLES1_0::~CRenderTargetTextureOGLES1_0()
{
}

ITexturePtr CRenderTargetTextureOGLES1_0::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGLES1_0());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
