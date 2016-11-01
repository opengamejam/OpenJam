//
//  CIndexBufferOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CINDEXBUFFEROGLES1_0_H
#define CINDEXBUFFEROGLES1_0_H

#include "CIndexBufferOGLBase.h"

namespace jam
{
    
class CIndexBufferOGLES1_0 : public CIndexBufferOGLBase
{
public:
    CIndexBufferOGLES1_0();
    virtual ~CIndexBufferOGLES1_0();
};
    
}; // namespace jam

#endif /* CINDEXBUFFEROGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
