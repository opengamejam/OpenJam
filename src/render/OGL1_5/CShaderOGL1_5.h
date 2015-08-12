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
    
    virtual unsigned int Id();
    virtual bool Compile(const std::string& source, ShaderType shaderType);
    virtual bool IsCompiled() const;
    virtual ShaderType Type() const;
};

}; // namespace jam

#endif	/* CSHADEROGL1_5_H */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */
