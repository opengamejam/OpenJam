//
//  CShaderSourceCommon.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "CShaderSourceCommon.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

const std::string CShaderSourceCommon::s_FragmentShader = MULTI_LINE_STRING(
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
                                                                            
    varying mediump vec3 VaryingLightDir;

    void main() {
        vec3 n = normalize(VaryingNormal);
        vec3 l = normalize(VaryingLightDir);

        float diffuse = max(dot(n, l), 0.0);

        vec4 color = texture2D(MainTexture0, VaryingTextureCoord);
        vec4 colorOut = clamp((diffuse * color), 0.0, 1.0);

        gl_FragColor = vec4(colorOut.rgb, color.a);
    }
                                                                            
\n #endif \n

\n #ifdef OGLES3\n

\n #endif \n);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceCommon::Fragment() const
{
    return s_FragmentShader;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
