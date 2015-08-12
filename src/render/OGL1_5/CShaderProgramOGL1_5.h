//
//  CShaderProgramOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CSHADERPROGRAMOGL1_5_H
#define	CSHADERPROGRAMOGL1_5_H

#include "IShaderProgram.h"

namespace jam
{

class CShaderProgramOGL1_5 : public IShaderProgram
{
public:
    CShaderProgramOGL1_5();
    virtual ~CShaderProgramOGL1_5();
    
    virtual void Bind();
    virtual void Unbind();
    
    virtual void AttachShader(IShaderPtr shader);
    virtual void DetachShader(IShader::ShaderType shaderType);
    virtual bool IsShaderAttached(IShader::ShaderType shaderType);
    virtual bool IsValid();
    virtual bool Link();
    virtual bool IsLinked() const;
    
    virtual unsigned int Attribute(const std::string& name);
    virtual unsigned int Uniform(const std::string& name);
    
    // Default locations
    virtual unsigned int VertexPosition();
    virtual unsigned int TextureCoord();
    virtual unsigned int VertexColor();
    virtual unsigned int MainTexture();
    virtual unsigned int MainColor();
    virtual unsigned int ProjectionMatrix();
    virtual unsigned int ModelMatrix();
    
    virtual unsigned int Texture(unsigned int index);
    virtual unsigned int DiffuseTexture();
    virtual unsigned int NormalTexture();
    virtual unsigned int SpecularTexture();
    virtual unsigned int EnvironmentTexture();
    
    
private:
    int m_ProectionMatrixHadle;
    int m_ModelMatrixHadle;
    int m_VertexCoordHandle;
    int m_TextureCoordHandle;
    int m_VertexColorHandle;
    int m_ColorHandle;
    std::vector<int> m_TextureDataHadle;
    
    bool m_IsLinked;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;
};

}; // namespace jam

#endif	/* CSHADERPROGRAMOGL1_5_H */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */
