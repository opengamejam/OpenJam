//
//  IShaderProgram.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef ISHADERPROGRAM_H
#define ISHADERPROGRAM_H

#include "IShader.h"
#include "CMath.h"

namespace jam
{
    
CLASS_PTR(IShader)
    
class IShaderProgram
{
    JAM_OBJECT_BASE
public:
    typedef std::map< int, std::vector<int> > TUniInt;
    typedef std::map< int, std::vector<float> > TUniFloat;
    typedef std::map< int, glm::mat4x4> TUniMatrix4Float;
    typedef std::list<TUniInt> TUniIntList;
    typedef std::list<TUniFloat> TUniFloatList;
    
public:
    IShaderProgram() = default;
    virtual ~IShaderProgram() = default;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
    virtual void AttachShader(IShaderPtr shader) = 0;
    virtual void DetachShader(IShader::ShaderType shaderType) = 0;
    virtual bool IsShaderAttached(IShader::ShaderType shaderType) = 0;
    virtual bool IsValid() = 0;
    virtual bool Link() = 0;
    virtual bool IsLinked() const = 0;
    
    virtual uint32_t Attribute(const std::string& name) = 0;
    virtual uint32_t Uniform(const std::string& name) = 0;
    
    virtual uint32_t VertexPosition() = 0;
    virtual uint32_t VertexNormal() = 0;
    virtual uint32_t VertexUV() = 0;
    virtual uint32_t VertexColor() = 0;
    virtual uint32_t MainTexture() = 0;
    virtual uint32_t MainColor() = 0;
    virtual uint32_t ProjectionMatrix() = 0;
    virtual uint32_t ModelMatrix() = 0;
    
    virtual uint32_t Texture(uint32_t index) = 0;
    virtual uint32_t DiffuseTexture() = 0;
    virtual uint32_t NormalTexture() = 0;
    virtual uint32_t SpecularTexture() = 0;
    virtual uint32_t EnvironmentTexture() = 0;
    
    virtual bool BindUniform1i(const std::string& uniform, int value) = 0;
    virtual bool BindUniform1f(const std::string& uniform, float value) = 0;
    virtual bool BindUniform2i(const std::string& uniform, int value1, int value2) = 0;
    virtual bool BindUniform2f(const std::string& uniform, float value1, float value2) = 0;
    virtual bool BindUniformfv(const std::string& uniform, const std::vector<float>& value) = 0;
    virtual bool BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value) = 0;
    
    virtual const TUniInt& Uniformsi() const = 0;
    virtual const TUniFloat& Uniformsf() const = 0;
    virtual const TUniFloat& Uniformsfv() const = 0;
    virtual const TUniMatrix4Float& UniformsMatrix4x4f() const = 0;
    virtual void UpdateUniforms() const = 0;
};
    
}; // namespace jam

#endif /* ISHADERPROGRAM_H */
