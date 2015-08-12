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

const std::string CShaderSourceSprite::s_FragmentShader = MULTI_LINE_STRING(
\n#ifdef OGL2_0\n
precision mediump float;

uniform sampler2D        MainTexture0;
uniform sampler2D        MainTexture1;
uniform sampler2D        MainTexture2;
uniform sampler2D        MainTexture3;
uniform sampler2D        MainTexture4;
uniform sampler2D        MainTexture5;
varying mediump vec2     VaryingTextureCoord;
varying mediump vec4     VaryingMainColor;

void main()
{
    vec4 color = texture2D(MainTexture0, VaryingTextureCoord);
    gl_FragColor = color;
}
\n#endif\n

\n#ifdef OGLES3\n

\n#endif\n
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