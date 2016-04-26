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

const std::string CShaderSourceCommon::s_GeomentryShader;
const std::string CShaderSourceCommon::s_VertexShader = MULTI_LINE_STRING(
\n#ifdef OGL2_0\n

uniform mediump vec3     LightDir;
                                                                          
varying mediump vec3     VaryingLightDir;
                                                                          
void main(void)
{
    mat4 MVMatrix = MainViewMatrix * MainModelMatrix;
    mat4 MVPMatrix = MainProjectionMatrix * MainModelMatrix;
    gl_Position = MVPMatrix * MainVertexPosition;
    
    VaryingNormal = vec3(MainNormalMatrix * vec4(MainVertexNormal, 0.0));
    VaryingTextureCoord = MainVertexUV;
    VaryingMainColor = MainVertexColor;
    
    vec4 vertexPos = MVMatrix * MainVertexPosition;
    vec4 lightPos = MVMatrix * vec4(LightDir, 0.0);
    
    VaryingLightDir = vec3(lightPos - vertexPos);
}
                                                                          
\n#endif\n

\n#ifdef OGLES3\n

\n#endif\n
);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceCommon::Vertex() const
{
    return s_VertexShader;
}

const std::string& CShaderSourceCommon::Geometry() const
{
    return s_GeomentryShader;
}

bool CShaderSourceCommon::Load(const std::string& filename)
{
    return true;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************