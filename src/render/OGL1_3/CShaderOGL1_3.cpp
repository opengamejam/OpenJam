//
//  CShaderOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CShaderOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGL1_3::CShaderOGL1_3()
{
}

CShaderOGL1_3::~CShaderOGL1_3()
{
}

uint32_t CShaderOGL1_3::Id()
{
    return 0;
}

bool CShaderOGL1_3::Compile(const std::string& source, ShaderType shaderType)
{
    return true;
}

bool CShaderOGL1_3::IsCompiled() const
{
    return true;
}

IShader::ShaderType CShaderOGL1_3::Type() const
{
    return IShader::Vertex;
}

const std::string& CShaderOGL1_3::Source() const
{
    static std::string dummySource;
    return dummySource;
}

void CShaderOGL1_3::AddDefinition(const std::string& identifier)
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) */
