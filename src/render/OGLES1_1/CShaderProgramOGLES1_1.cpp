//
//  CShaderProgramOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#include "CShaderProgramOGLES1_1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

uint32_t CShaderProgramOGLES1_1::s_NextLocation = 0;

CShaderProgramOGLES1_1::CShaderProgramOGLES1_1()
{
}

CShaderProgramOGLES1_1::~CShaderProgramOGLES1_1()
{
}

void CShaderProgramOGLES1_1::Bind()
{
}

void CShaderProgramOGLES1_1::Unbind()
{
}

void CShaderProgramOGLES1_1::AttachShader(IShaderPtr shader)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shader->Type());
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }

    m_AttachedShaders[shader->Type()] = shader;
}

void CShaderProgramOGLES1_1::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }
}

bool CShaderProgramOGLES1_1::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGLES1_1::Link()
{
    return true;
}

bool CShaderProgramOGLES1_1::IsLinked() const
{
    return true;
}

uint32_t CShaderProgramOGLES1_1::Attribute(const std::string& name)
{
    uint32_t location = CachedAttribute(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheAttribute(name, location);
    }
    
    return location;
}

uint32_t CShaderProgramOGLES1_1::Uniform(const std::string& name)
{
    uint32_t location = CachedUniform(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheUniform(name, location);
    }
    
    return location;
}

void CShaderProgramOGLES1_1::UpdateUniforms() const
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_1) */
