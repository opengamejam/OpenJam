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
\n #ifdef OGL2_0\n
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
    uniform mediump vec3 LightDir;
    varying mediump vec3 VaryingLightDir;

    void main(void) {
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
                                                                          
\n #endif \n

\n #ifdef OGLES3\n

\n #endif \n);

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

const std::string& CShaderSourceCommon::VertexAttributeName() const
{
    static std::string vertextAttributeName = "MainVertexPosition";
    return vertextAttributeName;
}

const std::string& CShaderSourceCommon::UVAttributeName() const
{
    static std::string uvAttributeName = "MainVertexUV";
    return uvAttributeName;
}

const std::string& CShaderSourceCommon::NormalAttributeName() const
{
    static std::string normalAttributeName = "MainVertexNormal";
    return normalAttributeName;
}

const std::string& CShaderSourceCommon::ColorAttributeName() const
{
    static std::string colorAttributeName = "MainVertexColor";
    return colorAttributeName;
}

const std::string& CShaderSourceCommon::ProjectionMatrixUniformName() const
{
    static std::string uniformName = "MainProjectionMatrix";
    return uniformName;
}

const std::string& CShaderSourceCommon::ViewMatrixUniformName() const
{
    static std::string uniformName = "MainViewMatrix";
    return uniformName;
}

const std::string& CShaderSourceCommon::ModelMatrixUniformName() const
{
    static std::string uniformName = "MainModelMatrix";
    return uniformName;
}

const std::string& CShaderSourceCommon::NormalMatrixUniformName() const
{
    static std::string uniformName = "MainNormalMatrix";
    return uniformName;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
