//
//  CShaderOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CSHADEROGLBASE_H
#define CSHADEROGLBASE_H

#include "IShader.h"

namespace jam {

class CShaderOGLBase : public IShader {
    JAM_OBJECT
public:
    CShaderOGLBase();
    virtual ~CShaderOGLBase();

    /*
     * OpenGL specific
     */
    virtual GLenum ShaderTypeToGlType(IShader::ShaderType type);
};

}; // namespace jam

#endif /* CSHADEROGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
