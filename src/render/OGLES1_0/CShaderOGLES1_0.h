//
//  CShaderOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#ifndef CSHADEROGLES1_0_H
#define CSHADEROGLES1_0_H

#include "CShaderOGLBase.h"

namespace jam {

class CShaderOGLES1_0 : public CShaderOGLBase {
    JAM_OBJECT
public:
    CShaderOGLES1_0();
    virtual ~CShaderOGLES1_0();

    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
    virtual const std::string& Source() const override;
    virtual void AddDefinition(const std::string& identifier) override;
};

}; // namespace jam

#endif /* CSHADEROGLES1_0_H */

#endif /* defined(RENDER_OGLES1_0) */
 
