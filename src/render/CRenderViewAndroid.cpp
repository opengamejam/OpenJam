//
//  CRenderViewAndroid.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_ANDROID)

#include "CRenderViewAndroid.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewAndroid::CRenderViewAndroid(unsigned int width, unsigned int height)
    : IRenderView(width, height)
{}

CRenderViewAndroid::~CRenderViewAndroid()
{
}

void CRenderViewAndroid::CreateView()
{
    
}

void CRenderViewAndroid::SwapBuffer() const
{
    
}

void CRenderViewAndroid::UpdateEvents() const
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_ANDROID */
