//
//  CShaderOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CShaderOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGL1_5::CShaderOGL1_5()
{
}

CShaderOGL1_5::~CShaderOGL1_5()
{
}

uint32_t CShaderOGL1_5::Id()
{
    return 0;
}

bool CShaderOGL1_5::Compile(const std::string& source, ShaderType shaderType)
{
    return true;
}

bool CShaderOGL1_5::IsCompiled() const
{
    return true;
}

IShader::ShaderType CShaderOGL1_5::Type() const
{
    return IShader::Vertex;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1