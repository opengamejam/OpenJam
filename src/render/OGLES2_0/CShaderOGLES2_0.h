//
//  CShaderOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#ifndef CSHADEROGLES2_0_H
#define CSHADEROGLES2_0_H

#include "CShaderOGLBase.h"

namespace jam {

class CShaderOGLES2_0 : public CShaderOGLBase {
public:
    CShaderOGLES2_0();
    virtual ~CShaderOGLES2_0();

    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
    virtual const std::string& Source() const override;
    virtual void AddDefinition(const std::string& identifier) override;

private:
    uint32_t m_Id;
    bool m_IsCompiled;
    ShaderType m_Type;
    std::string m_Source;
    std::list<std::string> m_Preproccessor;
};

}; // namespace jam

#endif /* CSHADEROGLES2_0_H */

#endif /* defined(RENDER_OGLES2_0) */
