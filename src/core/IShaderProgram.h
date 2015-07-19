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

namespace jam
{
    
CLASS_PTR(IShader);
    
class IShaderProgram
{
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
    
    virtual unsigned int Attribute(const std::string& name) = 0;
    virtual unsigned int Uniform(const std::string& name) = 0;
    
    virtual unsigned int VertexPosition() = 0;
    virtual unsigned int TextureCoord() = 0;
    virtual unsigned int VertexColor() = 0;
    virtual unsigned int MainTexture() = 0;
    virtual unsigned int MainColor() = 0;
    virtual unsigned int ProjectionMatrix() = 0;
    virtual unsigned int ModelMatrix() = 0;
    
    virtual unsigned int Texture(unsigned int index) = 0;
    virtual unsigned int DiffuseTexture() = 0;
    virtual unsigned int NormalTexture() = 0;
    virtual unsigned int SpecularTexture() = 0;
    virtual unsigned int EnvironmentTexture() = 0;
};
    
}; // namespace jam

#endif /* ISHADERPROGRAM_H */
