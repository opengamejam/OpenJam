//
//  CShaderProgramOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CSHADERPROGRAMOGLBASE_H
#define CSHADERPROGRAMOGLBASE_H

#include "IShaderProgram.h"

namespace jam {

class CShaderProgramOGLBase : public IShaderProgram {
public:
    CShaderProgramOGLBase();
    virtual ~CShaderProgramOGLBase();
};

}; // namespace jam

#endif /* CSHADERPROGRAMOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
