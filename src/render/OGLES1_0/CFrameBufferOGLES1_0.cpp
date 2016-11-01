//
//  CFrameBufferOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CFrameBufferOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGLES1_0::CFrameBufferOGLES1_0(uint32_t width, uint32_t height)
: CFrameBufferOGLBase(width, height)
{
}

CFrameBufferOGLES1_0::~CFrameBufferOGLES1_0()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // defined(RENDER_OGLES1_0)
