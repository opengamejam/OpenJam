//
//  CIndexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CINDEXBUFFEROGL1_3_H
#define CINDEXBUFFEROGL1_3_H

#include "CIndexBufferOGLBase.h"

namespace jam {

class CIndexBufferOGL1_3 : public CIndexBufferOGLBase {
    JAM_OBJECT
public:
    CIndexBufferOGL1_3();
    virtual ~CIndexBufferOGL1_3();
};

}; // namespace jam

#endif /* CINDEXBUFFEROGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
