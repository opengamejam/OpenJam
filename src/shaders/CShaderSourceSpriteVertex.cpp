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

const std::string CShaderSourceSprite::s_GeomentryShader;
const std::string CShaderSourceSprite::s_VertexShader = MULTI_LINE_STRING(
\n#ifdef OGL2_0\n
attribute highp vec4     MainPositionVertex;
attribute mediump vec2   MainTextureCoord;
attribute highp vec4     MainVertexColor;
uniform mediump mat4     MainProjectionMatrix;
uniform mediump mat4     MainModelMatrix;
varying mediump vec2     VaryingTextureCoord;
varying highp vec4       VaryingMainColor;

void main(void)
{
    gl_Position = MainPositionVertex;
    gl_Position *= MainModelMatrix;
    gl_Position *= MainProjectionMatrix;
    
    VaryingTextureCoord = MainTextureCoord;
    VaryingMainColor = MainVertexColor;
}
\n#endif\n

\n#ifdef OGLES3\n

\n#endif\n
);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceSprite::Vertex() const
{
    return s_VertexShader;
}

const std::string& CShaderSourceSprite::Geometry() const
{
    return s_GeomentryShader;
}

bool CShaderSourceSprite::Load(const std::string& filename)
{
    return true;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************