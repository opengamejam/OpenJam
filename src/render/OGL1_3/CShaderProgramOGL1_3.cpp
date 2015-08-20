//
//  CShaderProgramOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CShaderProgramOGL1_3.h"
#include "IMaterial.h"  // TODO: to remove

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramOGL1_3::CShaderProgramOGL1_3()
: m_ProectionMatrixHadle(-1)
, m_ModelMatrixHadle(-1)
, m_VertexCoordHandle(-1)
, m_TextureCoordHandle(-1)
, m_VertexColorHandle(-1)
, m_ColorHandle(-1)
, m_IsLinked(false)
{
    m_TextureDataHadle.resize(IMaterial::MaxSamplingTextures);
    for (size_t i = 0; i < m_TextureDataHadle.size(); ++i)
    {
        m_TextureDataHadle[i] = -1;
    }
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
    if (it != m_AttachedShaders.end())
    {
        m_AttachedShaders.erase(it);
    }
    
    m_AttachedShaders[shader->Type()] = shader;
}

void CShaderProgramOGL1_3::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end())
    {
        m_AttachedShaders.erase(it);
    }
}

bool CShaderProgramOGL1_3::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGL1_3::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) &&
            IsShaderAttached(IShader::Fragment));
}

bool CShaderProgramOGL1_3::Link()
{
    m_VertexCoordHandle    = Attribute("MainPositionVertex");
    m_TextureCoordHandle   = Attribute("MainTextureCoord");
    m_VertexColorHandle    = Attribute("MainVertexColor");
    
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

bool CShaderProgramOGL1_3::IsLinked() const
{
    return m_IsLinked;
}

unsigned int CShaderProgramOGL1_3::Attribute(const std::string& name)
{
    static std::unordered_map<std::string, int> attributes = {
        {"MainPositionVertex", 0},
        {"MainTextureCoord", 1},
        {"MainVertexColor", 2}
    };
    
    if (attributes.find(name) != attributes.end())
    {
        return attributes[name];
    }
    
    return -1;
}

unsigned int CShaderProgramOGL1_3::Uniform(const std::string& name)
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
    
    return -1;
}

unsigned int CShaderProgramOGL1_3::VertexPosition()
{
    return m_VertexCoordHandle;
}

unsigned int CShaderProgramOGL1_3::TextureCoord()
{
    return m_TextureCoordHandle;
}

unsigned int CShaderProgramOGL1_3::VertexColor()
{
    return m_VertexColorHandle;
}

unsigned int CShaderProgramOGL1_3::MainTexture()
{
    return m_TextureDataHadle[0];
}

unsigned int CShaderProgramOGL1_3::MainColor()
{
    return m_ColorHandle;
}

unsigned int CShaderProgramOGL1_3::ProjectionMatrix()
{
    return m_ProectionMatrixHadle;
}

unsigned int CShaderProgramOGL1_3::ModelMatrix()
{
    return m_ModelMatrixHadle;
}

unsigned int CShaderProgramOGL1_3::Texture(unsigned int index)
{
    if (index < IMaterial::MaxSamplingTextures)
    {
        return m_TextureDataHadle[index];
    }
    return -1;
}

unsigned int CShaderProgramOGL1_3::DiffuseTexture()
{
    return m_TextureDataHadle[0];
}

unsigned int CShaderProgramOGL1_3::NormalTexture()
{
    return m_TextureDataHadle[1];
}

unsigned int CShaderProgramOGL1_3::SpecularTexture()
{
    return m_TextureDataHadle[2];
}

unsigned int CShaderProgramOGL1_3::EnvironmentTexture()
{
    return m_TextureDataHadle[3];
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3
