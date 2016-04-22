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

const std::string CShaderSourceInsert::s_GeomentryShader;
const std::string CShaderSourceInsert::s_VertexShader = MULTI_LINE_STRING(
\n#ifdef OGL2_0\n

// Attributes
attribute mediump vec4   MainVertexPosition;
attribute mediump vec3   MainVertexNormal;
attribute mediump vec2   MainVertexUV;
attribute mediump vec4   MainVertexColor;

// Uniforms
uniform mediump mat4     MainProjectionMatrix;
uniform mediump mat4     MainViewMatrix;
uniform mediump mat4     MainModelMatrix;

// Varyings
varying mediump vec3     VaryingNormal;
varying mediump vec2     VaryingTextureCoord;
varying mediump vec4     VaryingMainColor;
                                                                          
\n#endif\n

\n#ifdef OGLES3\n

\n#endif\n
);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceInsert::Vertex() const
{
    return s_VertexShader;
}

const std::string& CShaderSourceInsert::Geometry() const
{
    return s_GeomentryShader;
}

bool CShaderSourceInsert::Load(const std::string& filename)
{
    return true;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************