//
//  CShaderProgramVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CShaderProgramVulkan.h"
#include "CShaderVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramVulkan::CShaderProgramVulkan(CRendererVulkanPtr renderer)
: m_Renderer(renderer)
, m_IsLinked(false)
{
}

CShaderProgramVulkan::~CShaderProgramVulkan()
{
}

void CShaderProgramVulkan::Bind()
{
}

void CShaderProgramVulkan::Unbind()
{
}

void CShaderProgramVulkan::AttachShader(IShaderPtr shader)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shader->Type());
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }

    m_AttachedShaders[shader->Type()] = shader;

    m_IsLinked = false;
}

void CShaderProgramVulkan::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end()) {
        m_AttachedShaders.erase(it);
    }

    m_IsLinked = false;
}

bool CShaderProgramVulkan::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramVulkan::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) && IsShaderAttached(IShader::Fragment));
}

bool CShaderProgramVulkan::Link()
{
    m_IsLinked = false;

    if (!m_IsLinked) {
        
    }

    //assert(glGetError() == GL_NO_ERROR);

    return m_IsLinked;
}

bool CShaderProgramVulkan::IsLinked() const
{
    return m_IsLinked;
}

uint32_t CShaderProgramVulkan::Attribute(const std::string& name)
{
    uint32_t location = std::numeric_limits<uint32_t>::max();
    std::unordered_map<std::string, uint32_t>::const_iterator it = m_CachedAttributes.find(name);
    if (it == m_CachedAttributes.end()) {
        // TODO
        m_CachedAttributes[name] = location;
    } else {
        location = it->second;
    }

    return location;
}

uint32_t CShaderProgramVulkan::Uniform(const std::string& name)
{
    uint32_t location = std::numeric_limits<uint32_t>::max();
    std::unordered_map<std::string, uint32_t>::const_iterator it = m_CachedUniforms.find(name);
    if (it == m_CachedUniforms.end()) {
        // TODO:
        m_CachedUniforms[name] = location;
    } else {
        location = it->second;
    }

    return location;
}

uint32_t CShaderProgramVulkan::ProjectionMatrix()
{
    return Uniform("MainProjectionMatrix");
}

uint32_t CShaderProgramVulkan::ModelMatrix()
{
    return Uniform("MainModelMatrix");
}

bool CShaderProgramVulkan::BindUniform1i(const std::string& uniform, int value)
{
    return BindUniform1i(Uniform(uniform), value);
}

bool CShaderProgramVulkan::BindUniform1f(const std::string& uniform, float value)
{
    return BindUniform1f(Uniform(uniform), value);
}

bool CShaderProgramVulkan::BindUniform2i(const std::string& uniform, int value1, int value2)
{
    return BindUniform2i(Uniform(uniform), value1, value2);
}

bool CShaderProgramVulkan::BindUniform2f(const std::string& uniform, float value1, float value2)
{
    return BindUniform2f(Uniform(uniform), value1, value2);
}

bool CShaderProgramVulkan::BindUniformfv(const std::string& uniform, const std::vector<float>& value)
{
    return BindUniformfv(Uniform(uniform), value);
}

bool CShaderProgramVulkan::BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value)
{
    return BindUniformMatrix4x4f(Uniform(uniform), value);
}

bool CShaderProgramVulkan::BindUniform1i(uint32_t location, int value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniInt[location] = { value };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramVulkan::BindUniform1f(uint32_t location, float value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloat[location] = { value };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramVulkan::BindUniform2i(uint32_t location, int value1, int value2)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniInt[location] = { value1, value2 };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramVulkan::BindUniform2f(uint32_t location, float value1, float value2)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloat[location] = { value1, value2 };
        
        return true;
    }
    
    return false;
}

bool CShaderProgramVulkan::BindUniformfv(uint32_t location, const std::vector<float>& value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniFloatVec[location] = value;
        
        return true;
    }
    
    return false;
}

bool CShaderProgramVulkan::BindUniformMatrix4x4f(uint32_t location, const glm::mat4x4& value)
{
    if (location != std::numeric_limits<uint32_t>::max()) {
        m_UniMatrixFloat[location] = value;
        
        return true;
    }
    
    return false;
}

const IShaderProgram::TUniInt& CShaderProgramVulkan::Uniformsi() const
{
    return m_UniInt;
}

const IShaderProgram::TUniFloat& CShaderProgramVulkan::Uniformsf() const
{
    return m_UniFloat;
}

const IShaderProgram::TUniFloat& CShaderProgramVulkan::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IShaderProgram::TUniMatrix4Float& CShaderProgramVulkan::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

void CShaderProgramVulkan::UpdateUniforms() const
{
    
}

std::vector<VkPipelineShaderStageCreateInfo> CShaderProgramVulkan::PiplineShaderStageInfos()
{
    std::vector<VkPipelineShaderStageCreateInfo> result;
    if (!IsValid()) {
        return result;
    }
    
    CShaderVulkanPtr vertex = m_AttachedShaders[IShader::Vertex]->Ptr<CShaderVulkan>();
    CShaderVulkanPtr fragment = m_AttachedShaders[IShader::Fragment]->Ptr<CShaderVulkan>();
    
    result.push_back(vertex->PipelineShaderStageCreateInfo());
    result.push_back(fragment->PipelineShaderStageCreateInfo());
    
    return result;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
