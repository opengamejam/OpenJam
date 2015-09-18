//
//  CShaderProgramOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CShaderProgramOGL2_0.h"
#include "IMaterial.h" // TODO: to remove

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramOGL2_0::CShaderProgramOGL2_0()
: IShaderProgram()
, m_ProgramObject(0)
, m_ProectionMatrixHadle(-1u)
, m_VertexCoordHandle(-1u)
, m_TextureCoordHandle(-1u)
, m_VertexColorHandle(-1u)
, m_ColorHandle(-1u)
, m_IsLinked(false)
{
    m_ProgramObject = glCreateProgram();
    
    m_TextureDataHadle.resize(IMaterial::MaxSamplingTextures);
    for (size_t i = 0; i < m_TextureDataHadle.size(); ++i)
    {
        m_TextureDataHadle[i] = -1u;
    }
}

CShaderProgramOGL2_0::~CShaderProgramOGL2_0()
{
}

void CShaderProgramOGL2_0::Bind()
{
    glUseProgram(m_ProgramObject);
}

void CShaderProgramOGL2_0::Unbind()
{
    glUseProgram(0);
}

void CShaderProgramOGL2_0::AttachShader(IShaderPtr shader)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shader->Type());
    if (it != m_AttachedShaders.end())
    {
        glDetachShader(m_ProgramObject, shader->Type());
        m_AttachedShaders.erase(it);
    }
    glAttachShader(m_ProgramObject, shader->Id());
    
    m_AttachedShaders[shader->Type()] = shader;
    
    m_IsLinked = false;
}

void CShaderProgramOGL2_0::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end())
    {
        glDetachShader(m_ProgramObject, shaderType);
        m_AttachedShaders.erase(it);
    }
    
    m_IsLinked = false;
}

bool CShaderProgramOGL2_0::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGL2_0::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) &&
            IsShaderAttached(IShader::Fragment));
}

bool CShaderProgramOGL2_0::Link()
{
    glLinkProgram(m_ProgramObject);
    
    GLint isLinked;
    glGetProgramiv(m_ProgramObject, GL_LINK_STATUS, &isLinked);
    m_IsLinked = (isLinked != 0);
    
    if (m_IsLinked)
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
    }
    else
    {
        GLint maxLength = 255;
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_ProgramObject, maxLength, &maxLength, &infoLog[0]);
        
        printf("%s", infoLog.data());
    }
    
    return m_IsLinked;
}

bool CShaderProgramOGL2_0::IsLinked() const
{
    return m_IsLinked;
}

uint32_t CShaderProgramOGL2_0::Attribute(const std::string& name)
{
    return glGetAttribLocation(m_ProgramObject, name.c_str());
}

uint32_t CShaderProgramOGL2_0::Uniform(const std::string& name)
{
    return glGetUniformLocation(m_ProgramObject, name.c_str());
}

uint32_t CShaderProgramOGL2_0::VertexPosition()
{
    return m_VertexCoordHandle;
}

uint32_t CShaderProgramOGL2_0::TextureCoord()
{
    return m_TextureCoordHandle;
}

uint32_t CShaderProgramOGL2_0::VertexColor()
{
    return m_VertexColorHandle;
}

uint32_t CShaderProgramOGL2_0::MainTexture()
{
    return m_TextureDataHadle[0];
}

uint32_t CShaderProgramOGL2_0::MainColor()
{
    return m_ColorHandle;
}

uint32_t CShaderProgramOGL2_0::ProjectionMatrix()
{
    return m_ProectionMatrixHadle;
}

uint32_t CShaderProgramOGL2_0::ModelMatrix()
{
    return m_ModelMatrixHadle;
}

uint32_t CShaderProgramOGL2_0::Texture(uint32_t index)
{
    if (index < IMaterial::MaxSamplingTextures)
    {
        return m_TextureDataHadle[index];
    }
    return -1u;
}

uint32_t CShaderProgramOGL2_0::DiffuseTexture()
{
    return m_TextureDataHadle[0];
}

uint32_t CShaderProgramOGL2_0::NormalTexture()
{
    return m_TextureDataHadle[1];
}

uint32_t CShaderProgramOGL2_0::SpecularTexture()
{
    return m_TextureDataHadle[2];
}

uint32_t CShaderProgramOGL2_0::EnvironmentTexture()
{
    return m_TextureDataHadle[3];
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0