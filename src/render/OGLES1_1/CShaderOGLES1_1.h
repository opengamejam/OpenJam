//
//  CShaderOGLES1_1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_1)

#ifndef CSHADEROGLES1_1_H
#define CSHADEROGLES1_1_H

#include "CShaderOGLBase.h"

namespace jam {

class CShaderOGLES1_1 : public CShaderOGLBase {
public:
    CShaderOGLES1_1();
    virtual ~CShaderOGLES1_1();

    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
    virtual const std::string& Source() const override;
    virtual void AddDefinition(const std::string& identifier) override;
};

}; // namespace jam

#endif /* CSHADEROGLES1_1_H */

#endif /* defined(RENDER_OGLES1_1) */


