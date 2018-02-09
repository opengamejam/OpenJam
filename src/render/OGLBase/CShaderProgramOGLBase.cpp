//
//  CShaderProgramOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CShaderProgramOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramOGLBase::CShaderProgramOGLBase()
{
}

CShaderProgramOGLBase::~CShaderProgramOGLBase()
{
}

bool CShaderProgramOGLBase::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) && IsShaderAttached(IShader::Fragment));
}

uint32_t CShaderProgramOGLBase::ProjectionMatrix()
{
    return Uniform("MainProjectionMatrix");
}

uint32_t CShaderProgramOGLBase::ModelMatrix()
{
    return Uniform("MainModelMatrix");
}

bool CShaderProgramOGLBase::BindUniform1i(const std::string& uniform, int value)
{
    return BindUniform1i(Uniform(uniform), value);
}

bool CShaderProgramOGLBase::BindUniform1f(const std::string& uniform, float value)
{
    return BindUniform1f(Uniform(uniform), value);
}

bool CShaderProgramOGLBase::BindUniform2i(const std::string& uniform, int value1, int value2)
{
    return BindUniform2i(Uniform(uniform), value1, value2);
}

bool CShaderProgramOGLBase::BindUniform2f(const std::string& uniform, float value1, float value2)
{
    return BindUniform2f(Uniform(uniform), value1, value2);
}

bool CShaderProgramOGLBase::BindUniformfv(const std::string& uniform, const std::vector<float>& value)
{
    return BindUniformfv(Uniform(uniform), value);
}

bool CShaderProgramOGLBase::BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value)
{
    return BindUniformMatrix4x4f(Uniform(uniform), value);
}

bool CShaderProgramOGLBase::BindUniform1i(uint32_t location, int value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniInt[location] = { value };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramOGLBase::BindUniform1f(uint32_t location, float value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloat[location] = { value };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramOGLBase::BindUniform2i(uint32_t location, int value1, int value2)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniInt[location] = { value1, value2 };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramOGLBase::BindUniform2f(uint32_t location, float value1, float value2)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloat[location] = { value1, value2 };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramOGLBase::BindUniformfv(uint32_t location, const std::vector<float>& value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloatVec[location] = value;
        
        return true;
    }
    
    return false;
}

bool CShaderProgramOGLBase::BindUniformMatrix4x4f(uint32_t location, const glm::mat4x4& value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniMatrixFloat[location] = value;
        
        return true;
    }
    
    return false;
}

const IShaderProgram::TUniInt& CShaderProgramOGLBase::Uniformsi() const
{
    return m_UniInt;
}

const IShaderProgram::TUniFloat& CShaderProgramOGLBase::Uniformsf() const
{
    return m_UniFloat;
}

const IShaderProgram::TUniFloat& CShaderProgramOGLBase::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IShaderProgram::TUniMatrix4Float& CShaderProgramOGLBase::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

uint32_t CShaderProgramOGLBase::CachedAttribute(const std::string& attribute) const
{
    uint32_t location = std::numeric_limits<uint32_t>::max();
    std::unordered_map<std::string, uint32_t>::const_iterator it = m_CachedAttributes.find(attribute);
    if (it != m_CachedAttributes.end()) {
        location = it->second;
    }
    
    return location;
}

void CShaderProgramOGLBase::CacheAttribute(const std::string& attribute, uint32_t location)
{
    m_CachedAttributes[attribute] = location;
}

uint32_t CShaderProgramOGLBase::CachedUniform(const std::string& uniform) const
{
    uint32_t location = std::numeric_limits<uint32_t>::max();
    std::unordered_map<std::string, uint32_t>::const_iterator it = m_CachedUniforms.find(uniform);
    if (it != m_CachedUniforms.end()) {
        location = it->second;
    }
    
    return location;
}

void CShaderProgramOGLBase::CacheUniform(const std::string& uniform, uint32_t location)
{
    m_CachedUniforms[uniform] = location;
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
