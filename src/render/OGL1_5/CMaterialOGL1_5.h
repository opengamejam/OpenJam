//
//  CMaterialOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#ifndef CMATERIALOGL1_5_H
#define CMATERIALOGL1_5_H

#include "CMaterialOGLBase.h"

namespace jam
{

class CMaterialOGL1_5 : public CMaterialOGLBase
{
public:
    CMaterialOGL1_5();
    virtual ~CMaterialOGL1_5();
    
    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALOGL1_5_H */

#endif /* defined(CMATERIALOGL1_5_H) */
