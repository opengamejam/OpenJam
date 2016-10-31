//
//  CFrameBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CFRAMEBUFFEROGL2_0_H
#define CFRAMEBUFFEROGL2_0_H

#include "CFrameBufferOGLBase.h"

namespace jam
{
    
class CFrameBufferOGL2_0 : public CFrameBufferOGLBase
{
public:
    CFrameBufferOGL2_0(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGL2_0();
};
    
}; // namespace jam

#endif /* CFRAMEBUFFEROGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
