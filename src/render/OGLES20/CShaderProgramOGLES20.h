//
//  CShaderProgramOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#ifndef CSHADERPROGRAMOGLES20_H
#define	CSHADERPROGRAMOGLES20_H

#include "IShaderProgram.h"

namespace jam
{

class CShaderProgramOGLES20 : public IShaderProgram
{
public:
    CShaderProgramOGLES20();
    virtual ~CShaderProgramOGLES20();
    
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
    unsigned int m_ProgramObject;
    
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

#endif	/* CSHADERPROGRAMOGLES20_H */

#endif /* RENDER_OGLES2 */
