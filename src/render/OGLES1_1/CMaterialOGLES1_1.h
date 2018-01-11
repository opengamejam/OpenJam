//
//  CMaterialOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CMATERIALOGLES1_1_H
#define CMATERIALOGLES1_1_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialOGLES1_1 : public CMaterialOGLBase {
    JAM_OBJECT
public:
    CMaterialOGLES1_1();
    virtual ~CMaterialOGLES1_1();

    /*
         * OpenGL specific
         */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func) override;
    virtual GLenum ConvertOperation(IMaterial::Operations op) override;
};

}; // namespace jam

#endif /* CMATERIALOGLES1_1_H */

#endif /* defined(CMATERIALOGLES1_1_H) */
