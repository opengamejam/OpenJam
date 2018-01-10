//
//  CIndexBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CINDEXBUFFEROGL1_5_H
#define CINDEXBUFFEROGL1_5_H

#include "CIndexBufferOGLBase.h"

namespace jam {

class CIndexBufferOGL1_5 : public CIndexBufferOGLBase {
    JAM_OBJECT
public:
    CIndexBufferOGL1_5();
    virtual ~CIndexBufferOGL1_5();
};

}; // namespace jam

#endif /* CINDEXBUFFEROGL1_5_H */

#endif /* defined(RENDER_OGL1_5) */
