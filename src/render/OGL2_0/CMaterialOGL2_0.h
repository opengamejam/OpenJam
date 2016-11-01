//
//  CMaterialOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CMATERIALOGL2_0_H
#define CMATERIALOGL2_0_H

#include "CMaterialOGLBase.h"

namespace jam {

class CMaterialOGL2_0 : public CMaterialOGLBase {
public:
    CMaterialOGL2_0();
    virtual ~CMaterialOGL2_0();

    /*
     * OpenGL specific
     */
    virtual GLenum ConvertTestFunc(IMaterial::TestFuncs func);
    virtual GLenum ConvertOperation(IMaterial::Operations op);
};

}; // namespace jam

#endif /* CMATERIALOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
