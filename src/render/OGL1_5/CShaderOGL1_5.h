//
//  CShaderOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CSHADEROGL1_5_H
#define	CSHADEROGL1_5_H

#include "IShader.h"

namespace jam
{

class CShaderOGL1_5 : public IShader
{
public:
    CShaderOGL1_5();
    virtual ~CShaderOGL1_5();
    
    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
};

}; // namespace jam

#endif	/* CSHADEROGL1_5_H */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */
