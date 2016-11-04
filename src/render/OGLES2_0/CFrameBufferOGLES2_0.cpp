//
//  CFrameBufferOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#include "CFrameBufferOGLES2_0.h"
#include "CRenderTargetColorOGLES2_0.h"
#include "CRenderTargetDepthOGLES2_0.h"
#include "CRenderTargetStencilOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CFrameBufferOGLES2_0::CFrameBufferOGLES2_0(uint32_t width, uint32_t height)
    : CFrameBufferOGLBase(width, height)
{
}

CFrameBufferOGLES2_0::~CFrameBufferOGLES2_0()
{
}

void CFrameBufferOGLES2_0::Bind() const
{
    CFrameBufferOGLBase::Bind();
}

void CFrameBufferOGLES2_0::Unbind() const
{
    CFrameBufferOGLBase::Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES2_0) */
