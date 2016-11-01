//
//  CFrameBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CFrameBufferOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGL2_0::CFrameBufferOGL2_0(uint32_t width, uint32_t height)
    : CFrameBufferOGLBase(width, height)
{
}

CFrameBufferOGL2_0::~CFrameBufferOGL2_0()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */
