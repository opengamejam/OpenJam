//
//  CRenderInstanceVulkan.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 11/8/17.
//  Copyright © 2017 Yevgeniy Logachev. All rights reserved.
//

#include "CRenderInstanceVulkan.h"

using namespace jam;

CRenderInstanceVulkan::CRenderInstanceVulkan(VkInstance instance,
                                             VkPhysicalDevice physicalDevice,
                                             VkSurfaceKHR surface,
                                             const std::vector<VkSurfaceFormatKHR>& surfaceFormats)
: m_IsInitialized(false)
, m_Instance(instance)
, m_PhysicalDevice(physicalDevice)
, m_Surface(surface)
, m_SurfaceFormats(surfaceFormats)
{
    
}

CRenderInstanceVulkan::~CRenderInstanceVulkan()
{
    
}

void CRenderInstanceVulkan::Initialize()
{
    if (IsInitialized()) {
        JAM_LOG("CRenderInstanceVulkan - trying to initialize twice")
        return;
    }
    
    m_IsInitialized = true;
}

void CRenderInstanceVulkan::Shutdown()
{
    if (!IsInitialized()) {
        JAM_LOG("CRenderInstanceVulkan - trying to shutdown not initialized object")
        return;
    }
    
    m_IsInitialized = false;
}

bool CRenderInstanceVulkan::IsInitialized() const
{
    return m_IsInitialized;
}

const VkInstance& CRenderInstanceVulkan::Instance() const
{
    return m_Instance;
}

const VkPhysicalDevice& CRenderInstanceVulkan::PhysicalDevice() const
{
    return m_PhysicalDevice;
}

const VkSurfaceKHR& CRenderInstanceVulkan::Surface() const
{
    return m_Surface;
}

const std::vector<VkSurfaceFormatKHR>& CRenderInstanceVulkan::SurfaceFormats() const
{
    return m_SurfaceFormats;
}
