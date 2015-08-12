//
//  CShaderOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CSHADEROGL1_3_H
#define	CSHADEROGL1_3_H

#include "IShader.h"

namespace jam
{

class CShaderOGL1_3 : public IShader
{
public:
    CShaderOGL1_3();
    virtual ~CShaderOGL1_3();
    
    virtual unsigned int Id();
    virtual bool Compile(const std::string& source, ShaderType shaderType);
    virtual bool IsCompiled() const;
    virtual ShaderType Type() const;
};

}; // namespace jam

#endif	/* CSHADEROGL1_3_H */

#endif /* RENDER_OGL1_3 */
