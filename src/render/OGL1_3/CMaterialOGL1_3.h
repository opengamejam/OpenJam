
//
//  CMaterialOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CMATERIALOGL1_3_H
#define CMATERIALOGL1_3_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialOGL1_3 : public CMaterialOGLBase {
    JAM_OBJECT
public:
    CMaterialOGL1_3();
    virtual ~CMaterialOGL1_3();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALOGL1_3_H */

#endif /* defined(CMATERIALOGL1_3_H) */
    
