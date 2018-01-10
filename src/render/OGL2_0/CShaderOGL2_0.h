//
//  CShaderOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#ifndef CSHADEROGL2_0_H
#define CSHADEROGL2_0_H

#include "CShaderOGLBase.h"

namespace jam {

class CShaderOGL2_0 : public CShaderOGLBase {
    JAM_OBJECT
public:
    CShaderOGL2_0();
    virtual ~CShaderOGL2_0();

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

#endif /* CSHADEROGL2_0_H */

#endif /* defined(RENDER_OGL2_0) */
