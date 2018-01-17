//
//  CShaderVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CShaderVulkan.h"
#include "CShaderSourceInsert.h"
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
    MLNShaderStage shaderStage = kMLNShaderStageAuto;
    switch (shaderType) {
        case Vertex:
            shaderStage = kMLNShaderStageVertex;
            break;
        case Fragment:
            shaderStage = kMLNShaderStageFragment;
            break;
        case Geometry:
            shaderStage = kMLNShaderStageGeometry;
            break;
            
        default:
            break;
    }
    
    bool compiled = m_Converter->convert(shaderStage, true, true);
    if (compiled) {
        VkShaderModuleCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = m_Converter->getSPIRV().size(),
            .pCode = reinterpret_cast<const uint32_t*>(m_Converter->getSPIRV().data())
        };
        
        CRendererVulkanPtr renderer = m_Renderer.lock();
        
        VkResult result = vkCreateShaderModule(renderer->LogicalDevice(), &createInfo, nullptr, &m_ShaderModule);
        if (result != VK_SUCCESS) {
            JAM_LOG("failed to create shader module!");
            compiled = false;
        }
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

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

//#endif /* defined(RENDER_VULKAN) */
 
