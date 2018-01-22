//
//  CShaderProgramOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CSHADERPROGRAMOGL2_0_H
#define CSHADERPROGRAMOGL2_0_H

#include "CShaderProgramOGLBase.h"

namespace jam {

class CShaderProgramOGL2_0 : public CShaderProgramOGLBase {
    JAM_OBJECT
public:
    CShaderProgramOGL2_0();
    virtual ~CShaderProgramOGL2_0();

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
    uint32_t m_ProgramObject;
    bool m_IsLinked;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;
};

}; // namespace jam

#endif /* CSHADERPROGRAMOGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */


