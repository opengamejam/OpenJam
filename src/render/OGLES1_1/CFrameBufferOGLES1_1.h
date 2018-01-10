//
//  CFrameBufferOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CFRAMEBUFFEROGLES1_1_H
#define CFRAMEBUFFEROGLES1_1_H

#include "CFrameBufferOGLBase.h"

namespace jam
{
    
class CFrameBufferOGLES1_1 : public CFrameBufferOGLBase
{
    JAM_OBJECT
public:
    CFrameBufferOGLES1_1(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGLES1_1();
};
    
}; // namespace jam

#endif /* CFRAMEBUFFEROGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
