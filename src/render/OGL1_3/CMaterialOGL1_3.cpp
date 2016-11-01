//
//  CMaterialOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CMaterialOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMaterialOGL1_3::CMaterialOGL1_3()
{
}

CMaterialOGL1_3::~CMaterialOGL1_3()
{
}

GLenum CMaterialOGL1_3::ConvertTestFunc(IMaterial::TestFuncs func)
{
    unsigned int stencilFunc = GL_NEVER;
    switch (func) {
    case IMaterial::Never:
        stencilFunc = GL_NEVER;
        break;

    case IMaterial::Less:
        stencilFunc = GL_LESS;
        break;

    case IMaterial::Equal:
        stencilFunc = GL_EQUAL;
        break;

    case IMaterial::LEqual:
        stencilFunc = GL_LEQUAL;
        break;

    case IMaterial::Greater:
        stencilFunc = GL_GREATER;
        break;

    case IMaterial::NotEqual:
        stencilFunc = GL_NOTEQUAL;
        break;

    case IMaterial::GEqual:
        stencilFunc = GL_GEQUAL;
        break;

    case IMaterial::Always:
        stencilFunc = GL_ALWAYS;
        break;
    };

    return stencilFunc;
}

GLenum CMaterialOGL1_3::ConvertOperation(IMaterial::Operations op)
{
#if !defined(OS_KOS)
    unsigned int operation = GL_KEEP;
    switch (op) {
    case IMaterial::Keep:
        operation = GL_KEEP;
        break;

    case IMaterial::Replace:
        operation = GL_REPLACE;
        break;

    case IMaterial::Incr:
        operation = GL_INCR;
        break;

    case IMaterial::Decr:
        operation = GL_DECR;
        break;

    case IMaterial::Invert:
        // TODO:
        break;

    case IMaterial::IncrWrap:
        // TODO:
        break;

    case IMaterial::DecrWrap:
        // TODO:
        break;
    };

    return operation;
#else
    return GL_REPLACE; // TODO: KOS
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) */
