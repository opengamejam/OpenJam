//
//  CShaderOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CSHADEROGL2_0_H
#define	CSHADEROGL2_0_H

#include "IShader.h"

namespace jam
{

class CShaderOGL2_0 : public IShader
{
public:
    CShaderOGL2_0();
    virtual ~CShaderOGL2_0();
    
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

#endif	/* CSHADEROGL2_0_H */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */