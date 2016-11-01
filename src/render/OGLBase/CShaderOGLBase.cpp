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

#include "CShaderOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLBase::CShaderOGLBase()
{
}

CShaderOGLBase::~CShaderOGLBase()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

GLenum CShaderOGLBase::ShaderTypeToGlType(IShader::ShaderType type)
{
    switch (type) {
    case IShader::Vertex:
        return GL_VERTEX_SHADER;
        break;

    case IShader::Fragment:
        return GL_FRAGMENT_SHADER;
        break;

    case IShader::Geometry:
        // TODO:
        break;
    }

    return GL_VERTEX_SHADER;
}

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
