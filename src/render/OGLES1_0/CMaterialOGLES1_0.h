//
//  CMaterialOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CMATERIALOGLES1_0_H
#define CMATERIALOGLES1_0_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialOGLES1_0 : public CMaterialOGLBase {
    JAM_OBJECT
public:
    CMaterialOGLES1_0();
    virtual ~CMaterialOGLES1_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALOGLES1_0_H */

#endif /* defined(CMATERIALOGLES1_0_H) */
