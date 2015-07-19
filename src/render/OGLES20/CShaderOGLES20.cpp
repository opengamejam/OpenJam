//
//  CShaderOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CShaderOGLES20.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

INL unsigned int ShaderTypeToGlType(IShader::ShaderType type);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLES20::CShaderOGLES20()
: IShader()
, m_Id(0)
, m_IsCompiled(false)
, m_Type(ShaderType::Vertex)
{
}

CShaderOGLES20::~CShaderOGLES20()
{
}

unsigned int CShaderOGLES20::Id()
{
    return m_Id;
}

bool CShaderOGLES20::Compile(const std::string& source, ShaderType shaderType)
{
    m_IsCompiled = false;
    
    unsigned int glType = ShaderTypeToGlType(shaderType);
    
    std::string sourceDefined = MULTI_LINE_STRING(\n#define OGLES2\n);
    sourceDefined.append(source);
    
    m_Id = glCreateShader(glType);
    const GLchar* glData = reinterpret_cast<const GLchar*>(sourceDefined.data());
    const GLint size = (GLint)sourceDefined.size();
    glShaderSource(m_Id, 1, &glData, &size);
    glCompileShader(m_Id);
    
    GLint status;
    glGetShaderiv(m_Id, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(m_Id, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(m_Id, maxLength, &maxLength, &errorLog[0]);
        
        printf("Shader compilation error: %s\n", &errorLog[0]);
    }
    else
    {
        m_IsCompiled = true;
        m_Type = shaderType;
    }
    
    return m_IsCompiled;
}

bool CShaderOGLES20::IsCompiled() const
{
    return m_IsCompiled;
}

IShader::ShaderType CShaderOGLES20::Type() const
{
    return m_Type;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

INL unsigned int ShaderTypeToGlType(IShader::ShaderType type)
{
    unsigned int glType = GL_VERTEX_SHADER;
    switch (type)
    {
        case IShader::Vertex:
            glType = GL_VERTEX_SHADER;
            break;
            
        case IShader::Fragment:
            glType = GL_FRAGMENT_SHADER;
            break;
            
        case IShader::Geometry:
            break;
    }
    
    return glType;
}

#endif // RENDER_OGLES2