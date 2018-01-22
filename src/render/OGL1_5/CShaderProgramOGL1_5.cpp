//
//  CShaderProgramOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#include "CShaderProgramOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

uint32_t CShaderProgramOGL1_5::s_NextLocation = 0;

CShaderProgramOGL1_5::CShaderProgramOGL1_5()
{
}

CShaderProgramOGL1_5::~CShaderProgramOGL1_5()
{
}

void CShaderProgramOGL1_5::Bind()
{
}

void CShaderProgramOGL1_5::Unbind()
{
}

void CShaderProgramOGL1_5::AttachShader(IShaderPtr shader)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shader->Type());
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }

    m_AttachedShaders[shader->Type()] = shader;
}

void CShaderProgramOGL1_5::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }
}

bool CShaderProgramOGL1_5::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGL1_5::Link()
{
    return true;
}

bool CShaderProgramOGL1_5::IsLinked() const
{
    return true;
}

uint32_t CShaderProgramOGL1_5::Attribute(const std::string& name)
{
    uint32_t location = CachedAttribute(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheAttribute(name, location);
    }
    
    return location;
}

uint32_t CShaderProgramOGL1_5::Uniform(const std::string& name)
{
    uint32_t location = CachedUniform(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = s_NextLocation++;
        CacheUniform(name, location);
    }
    
    return location;
}

void CShaderProgramOGL1_5::UpdateUniforms() const
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_5) */
