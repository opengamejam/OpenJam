//
//  CIndexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CINDEXBUFFEROGL2_0_H
#define CINDEXBUFFEROGL2_0_H

#include "CIndexBufferOGLBase.h"

namespace jam
{
    
class CIndexBufferOGL2_0 : public CIndexBufferOGLBase
{
public:
    CIndexBufferOGL2_0();
    virtual ~CIndexBufferOGL2_0();
};
    
}; // namespace jam

#endif /* CINDEXBUFFEROGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
