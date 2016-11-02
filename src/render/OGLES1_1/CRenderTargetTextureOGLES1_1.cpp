//
//  CRenderTargetTextureOGLES1_1.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGLES1_1)

#include "CRenderTargetTextureOGLES1_1.h"
#include "CTextureOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGLES1_1::CRenderTargetTextureOGLES1_1()
{
}

CRenderTargetTextureOGLES1_1::~CRenderTargetTextureOGLES1_1()
{
}

ITexturePtr CRenderTargetTextureOGLES1_1::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGLES1_1());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_1) */
