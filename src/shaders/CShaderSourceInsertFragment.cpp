//
//  CShaderSourceInsert.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "CShaderSourceInsert.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

const std::string CShaderSourceInsert::s_FragmentShader = MULTI_LINE_STRING(
\n #ifdef OGL2_0\n

        precision mediump float;

    // Uniforms
    uniform sampler2D MainTexture0;
    uniform sampler2D MainTexture1;
    uniform sampler2D MainTexture2;
    uniform sampler2D MainTexture3;
    uniform sampler2D MainTexture4;
    uniform sampler2D MainTexture5;

    // Varyings
    varying mediump vec3 VaryingNormal;
    varying mediump vec2 VaryingTextureCoord;
    varying mediump vec4 VaryingMainColor;

\n #endif \n

\n #ifdef OGLES3\n

\n #endif \n);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceInsert::Fragment() const
{
    return s_FragmentShader;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
