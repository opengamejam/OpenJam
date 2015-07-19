//
//  CShaderOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#ifndef CSHADEROGLES1_H
#define	CSHADEROGLES1_H

#include "IShader.h"

namespace jam
{

class CShaderOGLES1 : public IShader
{
public:
    CShaderOGLES1();
    virtual ~CShaderOGLES1();
    
    virtual unsigned int Id();
    virtual bool Compile(const std::string& source, ShaderType shaderType);
    virtual bool IsCompiled() const;
    virtual ShaderType Type() const;
};

}; // namespace jam

#endif	/* CSHADEROGLES1_H */

#endif /* RENDER_OGLES1 */
