//
//  CIndexBufferOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CINDEXBUFFEROGLES2_0_H
#define CINDEXBUFFEROGLES2_0_H

#include "CIndexBufferOGLBase.h"

namespace jam {

class CIndexBufferOGLES2_0 : public CIndexBufferOGLBase {
public:
    CIndexBufferOGLES2_0();
    virtual ~CIndexBufferOGLES2_0();
};

}; // namespace jam

#endif /* CINDEXBUFFEROGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
