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
const std::string CShaderSourceCommon::s_VertexShader = MULTI_LINE_STRING(#version 310 es\n
    layout(location = 0) in vec4   MainVertexPosition;\n
    //layout(location = 1) in vec3   MainVertexNormal;\n
    //layout(location = 2) in vec2   MainVertexUV;\n
    //layout(location = 3) in vec4   MainVertexColor;\n
    \n
    //layout(location = 1) out vec2     VaryingTextureCoord;\n
    //layout(location = 2) out vec4     VaryingMainColor;\n
    void main(void) {\n
        gl_Position = MainVertexPosition;\n
        \n
        //VaryingTextureCoord = MainVertexUV;\n
        //VaryingMainColor = MainVertexColor;\n
    }\n
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
