//
//  CShaderProgramOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CSHADERPROGRAMOGLES1_0_H
#define CSHADERPROGRAMOGLES1_0_H

#include "CShaderProgramOGLBase.h"

namespace jam {

class CShaderProgramOGLES1_0 : public CShaderProgramOGLBase {
    JAM_OBJECT
public:
    CShaderProgramOGLES1_0();
    virtual ~CShaderProgramOGLES1_0();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void AttachShader(IShaderPtr shader) override;
    virtual void DetachShader(IShader::ShaderType shaderType) override;
    virtual bool IsShaderAttached(IShader::ShaderType shaderType) override;
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

#endif /* CSHADERPROGRAMOGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
