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

CShaderProgramOGLES1_1::CShaderProgramOGLES1_1()
: m_ProectionMatrixHadle(-1u)
, m_ModelMatrixHadle(-1u)
, m_VertexCoordHandle(-1u)
, m_VertexNormalHandle(-1u)
, m_TextureCoordHandle(-1u)
, m_VertexColorHandle(-1u)
, m_ColorHandle(-1u)
, m_IsLinked(false)
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
    if (it != m_AttachedShaders.end())
    {
        m_AttachedShaders.erase(it);
    }
    
    m_AttachedShaders[shader->Type()] = shader;
}

void CShaderProgramOGLES1_1::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end())
    {
        m_AttachedShaders.erase(it);
    }
}

bool CShaderProgramOGLES1_1::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGLES1_1::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) &&
            IsShaderAttached(IShader::Fragment));
}

bool CShaderProgramOGLES1_1::Link()
{
    m_VertexCoordHandle    = Attribute("MainVertexPosition");
    m_TextureCoordHandle   = Attribute("MainVertexUV");
    m_VertexColorHandle    = Attribute("MainVertexColor");
    
    m_TextureDataHadle.resize(6);
    for (size_t i = 0; i < m_TextureDataHadle.size(); ++i)
    {
        m_TextureDataHadle[i] = -1u;
    }
    m_TextureDataHadle[0]  = Uniform("MainTexture0");
    m_TextureDataHadle[1]  = Uniform("MainTexture1");
    m_TextureDataHadle[2]  = Uniform("MainTexture2");
    m_TextureDataHadle[3]  = Uniform("MainTexture3");
    m_TextureDataHadle[4]  = Uniform("MainTexture4");
    m_TextureDataHadle[5]  = Uniform("MainTexture5");
    
    m_ColorHandle          = Uniform("MainColor");
    
    m_ProectionMatrixHadle = Uniform("MainProjectionMatrix");
    m_ModelMatrixHadle     = Uniform("MainModelMatrix");
    
    m_IsLinked = true;
    return m_IsLinked;
}

bool CShaderProgramOGLES1_1::IsLinked() const
{
    return m_IsLinked;
}

uint32_t CShaderProgramOGLES1_1::Attribute(const std::string& name)
{
    static std::unordered_map<std::string, int> attributes = {
        {"MainVertexPosition", 0},
        {"MainVertexUV", 1},
        {"MainVertexColor", 2}
    };
    
    if (attributes.find(name) != attributes.end())
    {
        return attributes[name];
    }
    
    return -1u;
}

uint32_t CShaderProgramOGLES1_1::Uniform(const std::string& name)
{
    static std::unordered_map<std::string, int> uniforms = {
        {"MainTexture0", 0},
        {"MainTexture1", 1},
        {"MainTexture2", 2},
        {"MainTexture3", 3},
        {"MainTexture4", 4},
        {"MainTexture5", 5},
        {"MainColor", 6},
        {"MainProjectionMatrix", 7},
        {"MainModelMatrix", 8},
    };
    
    if (uniforms.find(name) != uniforms.end())
    {
        return uniforms[name];
    }
    
    return -1u;
}

uint32_t CShaderProgramOGLES1_1::VertexPosition()
{
    return m_VertexCoordHandle;
}

uint32_t CShaderProgramOGLES1_1::VertexNormal()
{
    return m_VertexNormalHandle;
}

uint32_t CShaderProgramOGLES1_1::VertexUV()
{
    return m_TextureCoordHandle;
}

uint32_t CShaderProgramOGLES1_1::VertexColor()
{
    return m_VertexColorHandle;
}

uint32_t CShaderProgramOGLES1_1::MainTexture()
{
    return m_TextureDataHadle[0];
}

uint32_t CShaderProgramOGLES1_1::MainColor()
{
    return m_ColorHandle;
}

uint32_t CShaderProgramOGLES1_1::ProjectionMatrix()
{
    return m_ProectionMatrixHadle;
}

uint32_t CShaderProgramOGLES1_1::ModelMatrix()
{
    return m_ModelMatrixHadle;
}

uint32_t CShaderProgramOGLES1_1::Texture(uint32_t index)
{
    if (index < 5)
    {
        return m_TextureDataHadle[index];
    }
    return -1u;
}

uint32_t CShaderProgramOGLES1_1::DiffuseTexture()
{
    return m_TextureDataHadle[0];
}

uint32_t CShaderProgramOGLES1_1::NormalTexture()
{
    return m_TextureDataHadle[1];
}

uint32_t CShaderProgramOGLES1_1::SpecularTexture()
{
    return m_TextureDataHadle[2];
}

uint32_t CShaderProgramOGLES1_1::EnvironmentTexture()
{
    return m_TextureDataHadle[3];
}

bool CShaderProgramOGLES1_1::BindUniform1i(const std::string& uniform, int value)
{
    m_UniInt[Uniform(uniform)] = {value};
    
    return true;
}

bool CShaderProgramOGLES1_1::BindUniform1f(const std::string& uniform, float value)
{
    m_UniFloat[Uniform(uniform)] = {value};
    
    return true;
}

bool CShaderProgramOGLES1_1::BindUniform2i(const std::string& uniform, int value1, int value2)
{
    m_UniInt[Uniform(uniform)] = {value1, value2};
    
    return true;
}

bool CShaderProgramOGLES1_1::BindUniform2f(const std::string& uniform, float value1, float value2)
{
    m_UniFloat[Uniform(uniform)] = {value1, value2};
    
    return true;
}

bool CShaderProgramOGLES1_1::BindUniformfv(const std::string& uniform, const std::vector<float>& value)
{
    m_UniFloatVec[Uniform(uniform)] = value;
    
    return true;
}

bool CShaderProgramOGLES1_1::BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value)
{
    m_UniMatrixFloat[Uniform(uniform)] = value;
    
    return true;
}

const IShaderProgram::TUniInt& CShaderProgramOGLES1_1::Uniformsi() const
{
    return m_UniInt;
}

const IShaderProgram::TUniFloat& CShaderProgramOGLES1_1::Uniformsf() const
{
    return m_UniFloat;
}

const IShaderProgram::TUniFloat& CShaderProgramOGLES1_1::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IShaderProgram::TUniMatrix4Float& CShaderProgramOGLES1_1::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
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
