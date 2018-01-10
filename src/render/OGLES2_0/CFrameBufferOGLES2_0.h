//
//  CFrameBufferOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CFRAMEBUFFEROGLES2_0_H
#define CFRAMEBUFFEROGLES2_0_H

#include "CFrameBufferOGLBase.h"

namespace jam {

class CFrameBufferOGLES2_0 : public CFrameBufferOGLBase {
    JAM_OBJECT
public:
    CFrameBufferOGLES2_0(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGLES2_0();
    
    /*
     * Bind current framebuffer
     */
    virtual void Bind() const override;
    
    /*
     * Unbind framebuffer
     */
    virtual void Unbind() const override;
};

}; // namespace jam

#endif /* CFRAMEBUFFEROGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
