//
//  CShaderProgramOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CSHADERPROGRAMOGL2_0_H
#define	CSHADERPROGRAMOGL2_0_H

#include "CShaderProgramOGLBase.h"

namespace jam
{

class CShaderProgramOGL2_0 : public CShaderProgramOGLBase
{
public:
    CShaderProgramOGL2_0();
    virtual ~CShaderProgramOGL2_0();
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
    virtual void AttachShader(IShaderPtr shader) override;
    virtual void DetachShader(IShader::ShaderType shaderType) override;
    virtual bool IsShaderAttached(IShader::ShaderType shaderType) override;
    virtual bool IsValid() override;
    virtual bool Link() override;
    virtual bool IsLinked() const override;
    
    virtual uint32_t Attribute(const std::string& name) override;
    virtual uint32_t Uniform(const std::string& name) override;
    
    // Default locations
    virtual uint32_t VertexPosition() override;
    virtual uint32_t VertexNormal() override;
    virtual uint32_t VertexUV() override;
    virtual uint32_t VertexColor() override;
    virtual uint32_t MainTexture() override;
    virtual uint32_t MainColor() override;
    virtual uint32_t ProjectionMatrix() override;
    virtual uint32_t ModelMatrix() override;
    
    virtual uint32_t Texture(uint32_t index) override;
    virtual uint32_t DiffuseTexture() override;
    virtual uint32_t NormalTexture() override;
    virtual uint32_t SpecularTexture() override;
    virtual uint32_t EnvironmentTexture() override;
    
    virtual bool BindUniform1i(const std::string& uniform, int value) override;
    virtual bool BindUniform1f(const std::string& uniform, float value) override;
    virtual bool BindUniform2i(const std::string& uniform, int value1, int value2) override;
    virtual bool BindUniform2f(const std::string& uniform, float value1, float value2) override;
    virtual bool BindUniformfv(const std::string& uniform, const std::vector<float>& value) override;
    virtual bool BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value) override;
    
    virtual const TUniInt& Uniformsi() const override;
    virtual const TUniFloat& Uniformsf() const override;
    virtual const TUniFloat& Uniformsfv() const override;
    virtual const TUniMatrix4Float& UniformsMatrix4x4f() const override;
    virtual void UpdateUniforms() const override;
    
private:
    uint32_t m_ProgramObject;
    
    std::unordered_map<std::string, uint32_t> m_CachedAttributes;
    std::unordered_map<std::string, uint32_t> m_CachedUniforms;
    
    bool m_IsLinked;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;
    
    TUniInt m_UniInt;
    TUniFloat m_UniFloat;
    TUniInt m_UniIntVec;
    TUniFloat m_UniFloatVec;
    TUniMatrix4Float m_UniMatrixFloat;
};

}; // namespace jam

#endif	/* CSHADERPROGRAMOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
