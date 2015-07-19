//
//  CShaderOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CShaderOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLES1::CShaderOGLES1()
{
}

CShaderOGLES1::~CShaderOGLES1()
{
}

unsigned int CShaderOGLES1::Id()
{
    return 0;
}

bool CShaderOGLES1::Compile(const std::string& source, ShaderType shaderType)
{
    return true;
}

bool CShaderOGLES1::IsCompiled() const
{
    return true;
}

IShader::ShaderType CShaderOGLES1::Type() const
{
    return IShader::Vertex;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1