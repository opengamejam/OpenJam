//
//  CRenderTargetTextureOGL2_0.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_OGL2_0)

#include "CRenderTargetTextureOGL2_0.h"
#include "CTextureOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureOGL2_0::CRenderTargetTextureOGL2_0()
{
}

CRenderTargetTextureOGL2_0::~CRenderTargetTextureOGL2_0()
{
}

ITexturePtr CRenderTargetTextureOGL2_0::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureOGL2_0());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */
