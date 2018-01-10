//
//  CFrameBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CFRAMEBUFFEROGL1_3_H
#define CFRAMEBUFFEROGL1_3_H

#include "CFrameBufferOGLBase.h"

namespace jam {

class CFrameBufferOGL1_3 : public CFrameBufferOGLBase {
    JAM_OBJECT
public:
    CFrameBufferOGL1_3(uint32_t width, uint32_t height);
    virtual ~CFrameBufferOGL1_3();
};

}; // namespace jam

#endif /* CFRAMEBUFFEROGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
