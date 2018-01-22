//
//  CShaderProgramOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CSHADERPROGRAMOGL1_3_H
#define CSHADERPROGRAMOGL1_3_H

#include "CShaderProgramOGLBase.h"

namespace jam {

class CShaderProgramOGL1_3 : public CShaderProgramOGLBase {
    JAM_OBJECT
public:
    CShaderProgramOGL1_3();
    virtual ~CShaderProgramOGL1_3();

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

    virtual void UpdateUniforms() const override;

private:
    static uint32_t s_NextLocation;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;
};

}; // namespace jam

#endif /* CSHADERPROGRAMOGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
