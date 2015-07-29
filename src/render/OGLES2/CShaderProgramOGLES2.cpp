//
//  CShaderProgramOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CShaderProgramOGLES2.h"
#include "IMaterial.h" // TODO: to remove

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramOGLES2::CShaderProgramOGLES2()
: IShaderProgram()
, m_ProgramObject(0)
, m_ProectionMatrixHadle(-1)
, m_VertexCoordHandle(-1)
, m_TextureCoordHandle(-1)
, m_VertexColorHandle(-1)
, m_ColorHandle(-1)
, m_IsLinked(false)
{
    m_ProgramObject = glCreateProgram();
    
    m_TextureDataHadle.resize(IMaterial::MaxSamplingTextures);
    for (size_t i = 0; i < m_TextureDataHadle.size(); ++i)
    {
        m_TextureDataHadle[i] = -1;
    }
}

CShaderProgramOGLES2::~CShaderProgramOGLES2()
{
}

void CShaderProgramOGLES2::Bind()
{
    glUseProgram(m_ProgramObject);
}

void CShaderProgramOGLES2::Unbind()
{
    glUseProgram(0);
}

void CShaderProgramOGLES2::AttachShader(IShaderPtr shader)
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

void CShaderProgramOGLES2::DetachShader(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    if (it != m_AttachedShaders.end())
    {
        glDetachShader(m_ProgramObject, shaderType);
        m_AttachedShaders.erase(it);
    }
    
    m_IsLinked = false;
}

bool CShaderProgramOGLES2::IsShaderAttached(IShader::ShaderType shaderType)
{
    std::map<IShader::ShaderType, IShaderPtr>::const_iterator it = m_AttachedShaders.find(shaderType);
    return (it != m_AttachedShaders.end());
}

bool CShaderProgramOGLES2::IsValid()
{
    return (IsShaderAttached(IShader::Vertex) &&
            IsShaderAttached(IShader::Fragment));
}

bool CShaderProgramOGLES2::Link()
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
    
    return m_IsLinked;
}

bool CShaderProgramOGLES2::IsLinked() const
{
    return m_IsLinked;
}

unsigned int CShaderProgramOGLES2::Attribute(const std::string& name)
{
    return glGetAttribLocation(m_ProgramObject, name.c_str());
}

unsigned int CShaderProgramOGLES2::Uniform(const std::string& name)
{
    return glGetUniformLocation(m_ProgramObject, name.c_str());
}

unsigned int CShaderProgramOGLES2::VertexPosition()
{
    return m_VertexCoordHandle;
}

unsigned int CShaderProgramOGLES2::TextureCoord()
{
    return m_TextureCoordHandle;
}

unsigned int CShaderProgramOGLES2::VertexColor()
{
    return m_VertexColorHandle;
}

unsigned int CShaderProgramOGLES2::MainTexture()
{
    return m_TextureDataHadle[0];
}

unsigned int CShaderProgramOGLES2::MainColor()
{
    return m_ColorHandle;
}

unsigned int CShaderProgramOGLES2::ProjectionMatrix()
{
    return m_ProectionMatrixHadle;
}

unsigned int CShaderProgramOGLES2::ModelMatrix()
{
    return m_ModelMatrixHadle;
}

unsigned int CShaderProgramOGLES2::Texture(unsigned int index)
{
    if (index < IMaterial::MaxSamplingTextures)
    {
        return m_TextureDataHadle[index];
    }
    return -1;
}

unsigned int CShaderProgramOGLES2::DiffuseTexture()
{
    return m_TextureDataHadle[0];
}

unsigned int CShaderProgramOGLES2::NormalTexture()
{
    return m_TextureDataHadle[1];
}

unsigned int CShaderProgramOGLES2::SpecularTexture()
{
    return m_TextureDataHadle[2];
}

unsigned int CShaderProgramOGLES2::EnvironmentTexture()
{
    return m_TextureDataHadle[3];
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2