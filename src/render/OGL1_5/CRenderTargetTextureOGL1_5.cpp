//
//  CRenderTargetTextureOGL1_5.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_5)

#include "CRenderTargetTextureOGL1_5.h"
#include "CTextureOGLES1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL1_5::CRenderTargetTextureOGL1_5()
{
}

CRenderTargetTextureOGL1_5::~CRenderTargetTextureOGL1_5()
{
}

ITexturePtr CRenderTargetTextureOGL1_5::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGL1_5());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_5) */
