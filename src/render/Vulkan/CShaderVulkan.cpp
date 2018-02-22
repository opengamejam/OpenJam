//
//  CShaderVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CShaderVulkan.h"
#include <MoltenGLSLToSPIRVConverter/GLSLToSPIRVConverter.h>
#include "CRendererVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CShaderVulkan::CShaderVulkan(CRendererVulkanPtr renderer)
    : m_Converter(new molten::GLSLToSPIRVConverter())
    , m_Type(Vertex)
    , m_Renderer(renderer)
    , m_ShaderModule(nullptr)
{
    AddDefinition("Vulkan");
}

CShaderVulkan::~CShaderVulkan()
{
    delete m_Converter;
}

uint32_t CShaderVulkan::Id()
{
    return 0;
}

bool CShaderVulkan::Compile(const std::string& source, ShaderType shaderType)
{
    m_Converter->setGLSL(source);

    m_Type = shaderType;
    VkShaderStageFlagBits stageFlag = VK_SHADER_STAGE_VERTEX_BIT;
    MLNShaderStage shaderStage = kMLNShaderStageAuto;
    switch (shaderType) {
        case Vertex:
            shaderStage = kMLNShaderStageVertex;
            stageFlag = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case Fragment:
            shaderStage = kMLNShaderStageFragment;
            stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case Geometry:
            shaderStage = kMLNShaderStageGeometry;
            stageFlag = VK_SHADER_STAGE_GEOMETRY_BIT;
            break;
            
        default:
            break;
    }
    
    bool compiled = m_Converter->convert(shaderStage, true, true);
    if (compiled) {
        VkShaderModuleCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = m_Converter->getSPIRV().size(),
            .pCode = reinterpret_cast<const uint32_t*>(m_Converter->getSPIRV().data())
        };
        
        CRendererVulkanPtr renderer = m_Renderer.lock();
        
        VkResult result = vkCreateShaderModule(renderer->LogicalDevice(), &createInfo, nullptr, &m_ShaderModule);
        if (result != VK_SUCCESS) {
            JAM_LOG("Failed to create shader module!");
            compiled = false;
        }
        
        m_PiplineShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_PiplineShaderStageInfo.stage = stageFlag;
        m_PiplineShaderStageInfo.pNext = nullptr;
        m_PiplineShaderStageInfo.flags = 0;
        m_PiplineShaderStageInfo.module = m_ShaderModule;
        m_PiplineShaderStageInfo.pName = "name";
        m_PiplineShaderStageInfo.pSpecializationInfo = nullptr;
    } else {
        printf("Failed to compile shader: %s", m_Converter->getResultLog().c_str());
    }
    
    return compiled;
}

bool CShaderVulkan::IsCompiled() const
{
    return m_Converter->getWasConverted();
}

IShader::ShaderType CShaderVulkan::Type() const
{
    return m_Type;
}

const std::string& CShaderVulkan::Source() const
{
    return m_Converter->getGLSL();
}

void CShaderVulkan::AddDefinition(const std::string& identifier)
{
    
}

const VkShaderModule& CShaderVulkan::ShaderModule() const
{
    return m_ShaderModule;
}

const VkPipelineShaderStageCreateInfo& CShaderVulkan::PipelineShaderStageCreateInfo() const
{
    return m_PiplineShaderStageInfo;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
 
