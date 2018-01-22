//
//  CShaderProgramOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CShaderProgramOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

uint32_t CShaderProgramOGL1_3::s_NextLocation = 0;

CShaderProgramOGL1_3::CShaderProgramOGL1_3()
{
}

CShaderProgramOGL1_3::~CShaderProgramOGL1_3()
{
}

void CShaderProgramOGL1_3::Bind()
{
}

void CShaderProgramOGL1_3::Unbind()
{
}

void CShaderProgramOGL1_3::AttachShader(IShaderPtr shader)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shader->Type());
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }

    m_AttachedShaders[shader->Type()] = shader;
}

void CShaderProgramOGL1_3::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }
}

bool CShaderProgramOGL1_3::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGL1_3::Link()
{
    return true;
}

bool CShaderProgramOGL1_3::IsLinked() const
{
    return true;
}

uint32_t CShaderProgramOGL1_3::Attribute(const std::string& name)
{
    uint32_t location = CachedAttribute(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheAttribute(name, location);
    }
    
    return location;
}

uint32_t CShaderProgramOGL1_3::Uniform(const std::string& name)
{
    uint32_t location = CachedUniform(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheUniform(name, location);
    }
    
    return location;
}

void CShaderProgramOGL1_3::UpdateUniforms() const
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) */
