//
//  CShaderOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#include "CShaderOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLES1_1::CShaderOGLES1_1()
{
}

CShaderOGLES1_1::~CShaderOGLES1_1()
{
}

uint32_t CShaderOGLES1_1::Id()
{
    return 0;
}

bool CShaderOGLES1_1::Compile(const std::string& source, ShaderType shaderType)
{
    return true;
}

bool CShaderOGLES1_1::IsCompiled() const
{
    return true;
}

IShader::ShaderType CShaderOGLES1_1::Type() const
{
    return IShader::Vertex;
}

const std::string& CShaderOGLES1_1::Source() const
{
    static std::string dummySource;
    return dummySource;
}

void CShaderOGLES1_1::AddDefinition(const std::string& identifier)
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_1) */
