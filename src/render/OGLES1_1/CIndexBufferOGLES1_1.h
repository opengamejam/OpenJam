//
//  CIndexBufferOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CINDEXBUFFEROGLES1_1_H
#define CINDEXBUFFEROGLES1_1_H

#include "CIndexBufferOGLBase.h"

namespace jam
{
    
    class CIndexBufferOGLES1_1 : public CIndexBufferOGLBase
    {
    public:
        CIndexBufferOGLES1_1();
        virtual ~CIndexBufferOGLES1_1();
    };
    
}; // namespace jam

#endif /* CINDEXBUFFEROGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */
