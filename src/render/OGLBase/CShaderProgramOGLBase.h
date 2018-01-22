//
//  CShaderProgramOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CSHADERPROGRAMOGLBASE_H
#define CSHADERPROGRAMOGLBASE_H

#include "IShaderProgram.h"

namespace jam {

class CShaderProgramOGLBase : public IShaderProgram {
    JAM_OBJECT
public:
    CShaderProgramOGLBase();
    virtual ~CShaderProgramOGLBase();
    
    virtual bool IsValid() override;
    
    // Default locations
    virtual uint32_t VertexPosition() override;
    virtual uint32_t VertexNormal() override;
    virtual uint32_t VertexUV() override;
    virtual uint32_t VertexColor() override;
    virtual uint32_t MainTexture() override;
    virtual uint32_t MainColor() override;
    virtual uint32_t ProjectionMatrix() override;
    virtual uint32_t ModelMatrix() override;
    
    virtual uint32_t Texture(uint32_t index) override;
    virtual uint32_t DiffuseTexture() override;
    virtual uint32_t NormalTexture() override;
    virtual uint32_t SpecularTexture() override;
    virtual uint32_t EnvironmentTexture() override;
    
    virtual bool BindUniform1i(const std::string& uniform, int value) override;
    virtual bool BindUniform1f(const std::string& uniform, float value) override;
    virtual bool BindUniform2i(const std::string& uniform, int value1, int value2) override;
    virtual bool BindUniform2f(const std::string& uniform, float value1, float value2) override;
    virtual bool BindUniformfv(const std::string& uniform, const std::vector<float>& value) override;
    virtual bool BindUniformMatrix4x4f(const std::string& uniform, const glm::mat4x4& value) override;
    
    virtual bool BindUniform1i(uint32_t location, int value) override;
    virtual bool BindUniform1f(uint32_t location, float value) override;
    virtual bool BindUniform2i(uint32_t location, int value1, int value2) override;
    virtual bool BindUniform2f(uint32_t location, float value1, float value2) override;
    virtual bool BindUniformfv(uint32_t location, const std::vector<float>& value) override;
    virtual bool BindUniformMatrix4x4f(uint32_t location, const glm::mat4x4& value) override;
    
    virtual const TUniInt& Uniformsi() const override;
    virtual const TUniFloat& Uniformsf() const override;
    virtual const TUniFloat& Uniformsfv() const override;
    virtual const TUniMatrix4Float& UniformsMatrix4x4f() const override;
    
protected:
    uint32_t CachedAttribute(const std::string& attribute) const;
    void CacheAttribute(const std::string& attribute, uint32_t location);
    
    uint32_t CachedUniform(const std::string& uniform) const;
    void CacheUniform(const std::string& uniform, uint32_t location);
    
private:
    std::unordered_map<std::string, uint32_t> m_CachedAttributes;
    std::unordered_map<std::string, uint32_t> m_CachedUniforms;
    
    TUniInt m_UniInt;
    TUniFloat m_UniFloat;
    TUniInt m_UniIntVec;
    TUniFloat m_UniFloatVec;
    TUniMatrix4Float m_UniMatrixFloat;
};

}; // namespace jam

#endif /* CSHADERPROGRAMOGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
