//
//  CShaderProgramVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CSHADERPROGRAMVULKAN_H
#define CSHADERPROGRAMVULKAN_H

#include "CShaderProgramOGLBase.h"

namespace jam {
    
CLASS_PTR(CRendererVulkan)
CLASS_PTR(CShaderProgramVulkan)

class CShaderProgramVulkan : public CShaderProgramOGLBase
{
    JAM_OBJECT
public:
    CShaderProgramVulkan(CRendererVulkanPtr renderer);
    virtual ~CShaderProgramVulkan();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void AttachShader(IShaderPtr shader) override;
    virtual void DetachShader(IShader::ShaderType shaderType) override;
    virtual bool IsShaderAttached(IShader::ShaderType shaderType) override;
    virtual bool IsValid() override;
    virtual bool Link() override;
    virtual bool IsLinked() const override;

    virtual uint32_t Attribute(const std::string& name) override;
    virtual uint32_t Uniform(const std::string& name) override;

    // Default locations
    virtual uint32_t ProjectionMatrix() override;
    virtual uint32_t ModelMatrix() override;

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
    virtual void UpdateUniforms() const override;
    
    /*
     * Vulkan specific
     */
    std::vector<VkPipelineShaderStageCreateInfo> PiplineShaderStageInfos();

private:
    CRendererVulkanWeak m_Renderer;
    
    std::unordered_map<std::string, uint32_t> m_CachedAttributes;
    std::unordered_map<std::string, uint32_t> m_CachedUniforms;

    bool m_IsLinked;
    std::map<IShader::ShaderType, IShaderPtr> m_AttachedShaders;

    TUniInt m_UniInt;
    TUniFloat m_UniFloat;
    TUniInt m_UniIntVec;
    TUniFloat m_UniFloatVec;
    TUniMatrix4Float m_UniMatrixFloat;
};

}; // namespace jam

#endif /* CSHADERPROGRAMVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
