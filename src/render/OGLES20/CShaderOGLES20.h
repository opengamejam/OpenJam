//
//  CShaderOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#ifndef CSHADEROGLES20_H
#define	CSHADEROGLES20_H

#include "IShader.h"

namespace jam
{

class CShaderOGLES20 : public IShader
{
public:
    CShaderOGLES20();
    virtual ~CShaderOGLES20();
    
    virtual unsigned int Id();
    virtual bool Compile(const std::string& source, ShaderType shaderType);
    virtual bool IsCompiled() const;
    virtual ShaderType Type() const;
    
private:
    unsigned int m_Id;
    bool m_IsCompiled;
    ShaderType m_Type;
};

}; // namespace jam

#endif	/* CSHADEROGLES20_H */

#endif /* RENDER_OGLES2 */
