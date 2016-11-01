//
//  CShaderOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CShaderOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLES1_0::CShaderOGLES1_0()
{
}

CShaderOGLES1_0::~CShaderOGLES1_0()
{
}

uint32_t CShaderOGLES1_0::Id()
{
    return 0;
}

bool CShaderOGLES1_0::Compile(const std::string& source, ShaderType shaderType)
{
    return true;
}

bool CShaderOGLES1_0::IsCompiled() const
{
    return true;
}

IShader::ShaderType CShaderOGLES1_0::Type() const
{
    return IShader::Vertex;
}

const std::string& CShaderOGLES1_0::Source() const
{
    static std::string dummySource;
    return dummySource;
}

void CShaderOGLES1_0::AddDefinition(const std::string& identifier)
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
