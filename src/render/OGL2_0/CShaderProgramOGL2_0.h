//
//  CShaderProgramOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CSHADERPROGRAMOGL2_0_H
#define	CSHADERPROGRAMOGL2_0_H

#include "IShaderProgram.h"

namespace jam
{

class CShaderProgramOGL2_0 : public IShaderProgram
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
    virtual uint32_t TextureCoord() override;
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
    
private:
    uint32_t m_ProgramObject;
    
    uint32_t m_ProectionMatrixHadle;
    uint32_t m_ModelMatrixHadle;
    uint32_t m_VertexCoordHandle;
    uint32_t m_TextureCoordHandle;
    uint32_t m_VertexColorHandle;
    uint32_t m_ColorHandle;
    std::vector<uint32_t> m_TextureDataHadle;
    
    bool m_IsLinked;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;
};

}; // namespace jam

#endif	/* CSHADERPROGRAMOGL2_0_H */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */
