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
const std::string CShaderSourceSprite::s_VertexShader = MULTI_LINE_STRING(#version 310 es\n
    layout(location = 0) in vec4   MainVertexPosition;\n
    //layout(location = 1) in vec3   MainVertexNormal;\n
    //layout(location = 2) in vec2   MainVertexUV;\n
    //layout(location = 3) in vec4   MainVertexColor;\n
    \n
    //layout(location = 1) out vec2     VaryingTextureCoord;\n
    //layout(location = 2) out vec4     VaryingMainColor;\n
    \n
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

const std::string& CShaderSourceSprite::VertexAttributeName() const
{
    static std::string vertextAttributeName = "MainVertexPosition";
    return vertextAttributeName;
}

const std::string& CShaderSourceSprite::UVAttributeName() const
{
    static std::string uvAttributeName = "MainVertexUV";
    return uvAttributeName;
}

const std::string& CShaderSourceSprite::NormalAttributeName() const
{
    static std::string normalAttributeName = "MainVertexNormal";
    return normalAttributeName;
}

const std::string& CShaderSourceSprite::ColorAttributeName() const
{
    static std::string colorAttributeName = "MainVertexColor";
    return colorAttributeName;
}

const std::string& CShaderSourceSprite::ProjectionMatrixUniformName() const
{
    static std::string uniformName = "MainProjectionMatrix";
    return uniformName;
}

const std::string& CShaderSourceSprite::ViewMatrixUniformName() const
{
    static std::string uniformName = "MainViewMatrix";
    return uniformName;
}

const std::string& CShaderSourceSprite::ModelMatrixUniformName() const
{
    static std::string uniformName = "MainModelMatrix";
    return uniformName;
}

const std::string& CShaderSourceSprite::NormalMatrixUniformName() const
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
