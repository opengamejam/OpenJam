//
//  CShaderOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CSHADEROGL1_3_H
#define CSHADEROGL1_3_H

#include "CShaderOGLBase.h"

namespace jam {

class CShaderOGL1_3 : public CShaderOGLBase {
public:
    CShaderOGL1_3();
    virtual ~CShaderOGL1_3();

    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
    virtual const std::string& Source() const override;
    virtual void AddDefinition(const std::string& identifier) override;
};

}; // namespace jam

#endif /* CSHADEROGL1_3_H */

#endif /* defined(RENDER_OGL1_3) */
