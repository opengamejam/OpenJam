//
//  CShaderSourceSprite.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "CShaderSourceSprite.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

const std::string CShaderSourceSprite::s_FragmentShader = MULTI_LINE_STRING(#version 310 es\n
    precision lowp float;\n
    //layout(binding = 0) uniform sampler2D MainTexture0;\n
    \n
    //layout(location = 1) in vec2 VaryingTextureCoord;\n
    //layout(location = 2) in vec4 VaryingMainColor;\n
    \n
    layout(location = 0) out vec4 outColor;\n
    \n
    void main() {\n
        vec4 color = vec4(1.0, 0.0, 0.0, 1.0);//texture(MainTexture0, VaryingTextureCoord);\n
        outColor = color;\n
    }\n
);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceSprite::Fragment() const
{
    return s_FragmentShader;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
