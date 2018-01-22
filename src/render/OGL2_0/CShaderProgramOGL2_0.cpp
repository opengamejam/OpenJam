//
//  CShaderProgramOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CShaderProgramOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderProgramOGL2_0::CShaderProgramOGL2_0()
    : m_ProgramObject(0)
    , m_IsLinked(false)
{
    m_ProgramObject = glCreateProgram();
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
    if (it != m_AttachedShaders.end()) {
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
    if (it != m_AttachedShaders.end()) {
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

bool CShaderProgramOGL2_0::Link()
{
    glLinkProgram(m_ProgramObject);

    GLint isLinked;
    glGetProgramiv(m_ProgramObject, GL_LINK_STATUS, &isLinked);
    m_IsLinked = (isLinked != 0);

    if (!m_IsLinked) {
        GLint maxLength = 255;
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_ProgramObject, maxLength, &maxLength, &infoLog[0]);

        printf("%s", infoLog.data());
    }

    //assert(glGetError() == GL_NO_ERROR);

    return m_IsLinked;
}

bool CShaderProgramOGL2_0::IsLinked() const
{
    return m_IsLinked;
}

uint32_t CShaderProgramOGL2_0::Attribute(const std::string& name)
{
    uint32_t location = CachedAttribute(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = glGetAttribLocation(m_ProgramObject, name.c_str());
        CacheAttribute(name, location);
    }
    
    return location;
}

uint32_t CShaderProgramOGL2_0::Uniform(const std::string& name)
{
    uint32_t location = CachedUniform(name);
    if (location == std::numeric_limits<uint32_t>::max()) {
        location = glGetUniformLocation(m_ProgramObject, name.c_str());
        CacheUniform(name, location);
    }
    
    return location;
}

INL void AddUniformMatrix4f(unsigned int location, const glm::mat4x4& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

INL void AddUniformf(unsigned int location, const std::vector<float>& value, bool isVector)
{
    if (isVector) {
        switch (value.size()) {
        case 1:
            glUniform1fv(location, 1, value.data());
            break;

        case 2:
            glUniform2fv(location, 1, value.data());
            break;

        case 3:
            glUniform3fv(location, 1, value.data());
            break;

        case 4:
            glUniform4fv(location, 1, value.data());
            break;
        };
    } else {
        switch (value.size()) {
        case 1:
            glUniform1f(location, value[0]);
            break;

        case 2:
            glUniform2f(location, value[0], value[1]);
            break;

        case 3:
            glUniform3f(location, value[0], value[1], value[2]);
            break;

        case 4:
            glUniform4f(location, value[0], value[1], value[2], value[3]);
            break;
        };
    }
}

INL void AddUniformi(unsigned int location, const std::vector<int>& value, bool isVector)
{
    if (isVector) {
        switch (value.size()) {
        case 1:
            glUniform1iv(location, 1, value.data());
            break;

        case 2:
            glUniform2iv(location, 1, value.data());
            break;

        case 3:
            glUniform3iv(location, 1, value.data());
            break;

        case 4:
            glUniform4iv(location, 1, value.data());
            break;
        };
    } else {
        switch (value.size()) {
        case 1:
            glUniform1i(location, value[0]);
            break;

        case 2:
            glUniform2i(location, value[0], value[1]);
            break;

        case 3:
            glUniform3i(location, value[0], value[1], value[2]);
            break;
        case 4:
            glUniform4i(location, value[0], value[1], value[2], value[3]);
            break;
        };
    }
}

INL void AddUniformMatrix4f(const IShaderProgram::TUniMatrix4Float& uniMatrix)
{
    std::for_each(uniMatrix.begin(), uniMatrix.end(), [&](const IShaderProgram::TUniMatrix4Float::value_type& value) {
        AddUniformMatrix4f(value.first, value.second);
    });
}

INL void AddUniformf(const IShaderProgram::TUniFloat& uniFloat, bool isVector)
{
    std::for_each(uniFloat.begin(), uniFloat.end(), [&](const IShaderProgram::TUniFloat::value_type& value) {
        AddUniformf(value.first, value.second, isVector);
    });
}

INL void AddUniformi(const IShaderProgram::TUniInt& uniInt, bool isVector)
{
    std::for_each(uniInt.begin(), uniInt.end(), [&](const IShaderProgram::TUniInt::value_type& value) {
        AddUniformi(value.first, value.second, isVector);
    });
}

void CShaderProgramOGL2_0::UpdateUniforms() const
{
    const IShaderProgram::TUniFloat& uniFloat = Uniformsf();
    const IShaderProgram::TUniInt& uniInt = Uniformsi();
    const IShaderProgram::TUniFloat& uniFloatVec = Uniformsfv();
    const IShaderProgram::TUniMatrix4Float& uniMatrix4x4f = UniformsMatrix4x4f();

    AddUniformi(uniInt, false);
    AddUniformf(uniFloat, false);
    AddUniformf(uniFloatVec, true);
    AddUniformMatrix4f(uniMatrix4x4f);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */
