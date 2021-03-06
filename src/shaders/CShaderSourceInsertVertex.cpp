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
\n
// Attributes\n
attribute mediump vec4   MainVertexPosition;\n
attribute mediump vec3   MainVertexNormal;\n
attribute mediump vec2   MainVertexUV;\n
attribute mediump vec4   MainVertexColor;\n
\n
// Uniforms\n
uniform mediump mat4     MainProjectionMatrix;\n
uniform mediump mat4     MainViewMatrix;\n
uniform mediump mat4     MainModelMatrix;\n
uniform mediump mat4     MainNormalMatrix;\n
\n
// Varyings\n
varying mediump vec3     VaryingNormal;\n
varying mediump vec2     VaryingTextureCoord;\n
varying mediump vec4     VaryingMainColor;\n
\n                              
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
