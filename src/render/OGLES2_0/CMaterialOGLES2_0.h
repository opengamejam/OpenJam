//
//  CMaterialOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CMATERIALOGLES2_0_H
#define CMATERIALOGLES2_0_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialOGLES2_0 : public CMaterialOGLBase {
    JAM_OBJECT
public:
    CMaterialOGLES2_0();
    virtual ~CMaterialOGLES2_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALOGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
