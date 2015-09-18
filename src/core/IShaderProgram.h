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
    
    virtual uint32_t Attribute(const std::string& name) = 0;
    virtual uint32_t Uniform(const std::string& name) = 0;
    
    virtual uint32_t VertexPosition() = 0;
    virtual uint32_t TextureCoord() = 0;
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
};
    
}; // namespace jam

#endif /* ISHADERPROGRAM_H */
