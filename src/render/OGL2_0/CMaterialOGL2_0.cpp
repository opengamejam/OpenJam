//
//  CMaterialOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CMaterialOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMaterialOGL2_0::CMaterialOGL2_0()
{
}

CMaterialOGL2_0::~CMaterialOGL2_0()
{
}

GLenum CMaterialOGL2_0::ConvertTestFunc(IMaterial::TestFuncs func)
{
    unsigned int stencilFunc = GL_NEVER;
    switch (func)
    {
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

GLenum CMaterialOGL2_0::ConvertOperation(IMaterial::Operations op)
{
    unsigned int operation = GL_KEEP;
    switch (op)
    {
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
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */
