//
//  CFrameBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CFRAMEBUFFEROGL1_5_H
#define CFRAMEBUFFEROGL1_5_H

#include "CFrameBufferOGLBase.h"

namespace jam {

class CFrameBufferOGL1_5 : public CFrameBufferOGLBase {
    JAM_OBJECT
public:
    CFrameBufferOGL1_5(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGL1_5();
};

}; // namespace jam

#endif /* CFRAMEBUFFEROGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */

