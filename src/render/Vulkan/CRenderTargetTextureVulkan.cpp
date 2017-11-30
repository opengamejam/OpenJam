//
//  CRenderTargetTextureVulkan.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/2/16.
//
//
#if defined(RENDER_VULKAN)

#include "CRenderTargetTextureVulkan.h"
#include "CTextureVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderTargetTextureVulkan::CRenderTargetTextureVulkan()
{
}

CRenderTargetTextureVulkan::~CRenderTargetTextureVulkan()
{
}

ITexturePtr CRenderTargetTextureVulkan::CreateTextureObject() const
{
    ITexturePtr texture(new CTextureVulkan());
    return texture;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_VULKAN) */
