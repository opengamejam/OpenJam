//
//  CShaderOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#include "CShaderOGLES2_0.h"
#include "CShaderSourceInsert.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderOGLES2_0::CShaderOGLES2_0()
    : m_Id(0)
    , m_IsCompiled(false)
    , m_Type(ShaderType::Vertex)
{
    AddDefinition("OGL2_0");
}

CShaderOGLES2_0::~CShaderOGLES2_0()
{
}

uint32_t CShaderOGLES2_0::Id()
{
    return m_Id;
}

bool CShaderOGLES2_0::Compile(const std::string& source, ShaderType shaderType)
{
    m_IsCompiled = false;

    uint32_t glType = ShaderTypeToGlType(shaderType);

    CShaderSourceInsert insertSource;

    // Add preproccessor's definitions
    m_Source = "";
    std::for_each(m_Preproccessor.begin(), m_Preproccessor.end(), [&](const std::string& identifier) {
        std::string definition;
        definition = std::string("\n#define ") + identifier + std::string("\n");

        m_Source.append(definition);
    });

    // Insert common uniforms and attributes
    if (shaderType == IShader::Vertex) {
        m_Source.append(insertSource.Vertex());
    } else if (shaderType == IShader::Fragment) {
        m_Source.append(insertSource.Fragment());
    } else if (shaderType == IShader::Geometry) {
        m_Source.append(insertSource.Geometry());
    }
    m_Source.append(source);

    m_Id = glCreateShader(glType);
    const GLchar* glData = reinterpret_cast<const GLchar*>(m_Source.data());
    const GLint size = (GLint)m_Source.size();
    glShaderSource(m_Id, 1, &glData, &size);
    glCompileShader(m_Id);

    GLint status;
    glGetShaderiv(m_Id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(m_Id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(m_Id, maxLength, &maxLength, &errorLog[0]);

        printf("Shader compilation error: %s\n", &errorLog[0]);
    } else {
        m_IsCompiled = true;
        m_Type = shaderType;
    }

    //assert(glGetError() == GL_NO_ERROR);

    return m_IsCompiled;
}

bool CShaderOGLES2_0::IsCompiled() const
{
    return m_IsCompiled;
}

IShader::ShaderType CShaderOGLES2_0::Type() const
{
    return m_Type;
}

const std::string& CShaderOGLES2_0::Source() const
{
    return m_Source;
}

void CShaderOGLES2_0::AddDefinition(const std::string& identifier)
{
    m_Preproccessor.push_back(identifier);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES2_0) */
 
