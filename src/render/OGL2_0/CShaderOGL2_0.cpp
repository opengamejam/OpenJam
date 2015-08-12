//
//  CShaderOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CShaderOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL unsigned int ShaderTypeToGlType(IShader::ShaderType type);

CShaderOGL2_0::CShaderOGL2_0()
: IShader()
, m_Id(0)
, m_IsCompiled(false)
, m_Type(ShaderType::Vertex)
{
}

CShaderOGL2_0::~CShaderOGL2_0()
{
}

unsigned int CShaderOGL2_0::Id()
{
    return m_Id;
}

bool CShaderOGL2_0::Compile(const std::string& source, ShaderType shaderType)
{
    m_IsCompiled = false;
    
    unsigned int glType = ShaderTypeToGlType(shaderType);
    
    std::string sourceDefined = MULTI_LINE_STRING(\n#define OGL2_0\n);
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

bool CShaderOGL2_0::IsCompiled() const
{
    return m_IsCompiled;
}

IShader::ShaderType CShaderOGL2_0::Type() const
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

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0