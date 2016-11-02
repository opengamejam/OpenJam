//
//  CRenderTargetTextureOGL1_3.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL1_3)

#include "CRenderTargetTextureOGL1_3.h"
#include "CTextureOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL1_3::CRenderTargetTextureOGL1_3()
{
}

CRenderTargetTextureOGL1_3::~CRenderTargetTextureOGL1_3()
{
}

ITexturePtr CRenderTargetTextureOGL1_3::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGL1_3());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) */
