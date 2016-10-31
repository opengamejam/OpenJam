//
//  CFrameBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#include "CFrameBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGL1_5::CFrameBufferOGL1_5(uint32_t width, uint32_t height)
: CFrameBufferOGLBase(width, height)
{
}

CFrameBufferOGL1_5::~CFrameBufferOGL1_5()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // defined(RENDER_OGL1_5)
