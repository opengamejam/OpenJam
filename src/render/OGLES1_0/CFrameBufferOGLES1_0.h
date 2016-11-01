//
//  CFrameBufferOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CFRAMEBUFFEROGLES1_0_H
#define CFRAMEBUFFEROGLES1_0_H

#include "CFrameBufferOGLBase.h"

namespace jam
{
    
class CFrameBufferOGLES1_0 : public CFrameBufferOGLBase
{
public:
    CFrameBufferOGLES1_0(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGLES1_0();
};
    
}; // namespace jam

#endif /* CFRAMEBUFFEROGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
