//
//  CFrameBufferOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#include "CFrameBufferOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGLES1_1::CFrameBufferOGLES1_1(uint32_t width, uint32_t height)
: CFrameBufferOGLBase(width, height)
{
}

CFrameBufferOGLES1_1::~CFrameBufferOGLES1_1()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // defined(RENDER_OGLES1_1)
