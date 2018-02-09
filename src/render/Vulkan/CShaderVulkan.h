//
//  CShaderVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CSHADERVULKAN_H
#define CSHADERVULKAN_H

#include "IShader.h"

namespace molten {
    class GLSLToSPIRVConverter;
};

namespace jam {
    
CLASS_PTR(CRendererVulkan)
CLASS_PTR(CShaderVulkan)

class CShaderVulkan : public IShader
{
    JAM_OBJECT
public:
    CShaderVulkan(CRendererVulkanPtr renderer);
    virtual ~CShaderVulkan();

    virtual uint32_t Id() override;
    virtual bool Compile(const std::string& source, ShaderType shaderType) override;
    virtual bool IsCompiled() const override;
    virtual ShaderType Type() const override;
    virtual const std::string& Source() const override;
    virtual void AddDefinition(const std::string& identifier) override;
    
    /*
     * Vulkan specific
     */
    const VkShaderModule& ShaderModule() const;
    const VkPipelineShaderStageCreateInfo& PipelineShaderStageCreateInfo() const;

private:
    molten::GLSLToSPIRVConverter *m_Converter;
    ShaderType m_Type;
    
    CRendererVulkanWeak m_Renderer;
    VkShaderModule m_ShaderModule;
    VkPipelineShaderStageCreateInfo m_PiplineShaderStageInfo;
};

}; // namespace jam

#endif /* CSHADERVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
