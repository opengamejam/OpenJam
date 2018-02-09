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
    \n
    void main(void) {
        mat4 MVMatrix = MainViewMatrix * MainModelMatrix;
        mat4 MVPMatrix = MainProjectionMatrix * MainModelMatrix;
        gl_Position = MVPMatrix * MainVertexPosition;
        
        VaryingNormal = vec3(MainNormalMatrix * vec4(MainVertexNormal, 0.0));
        VaryingTextureCoord = MainVertexUV;
        VaryingMainColor = MainVertexColor;
    }
\n #endif \n

\n #ifdef OGLES3\n

\n #endif \n);

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
