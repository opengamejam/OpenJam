//
//  CIndexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CINDEXBUFFERVULKAN_H
#define CINDEXBUFFERVULKAN_H

#include "IIndexBuffer.h"

namespace jam {

CLASS_PTR(CRendererVulkan)
    
class CIndexBufferVulkan : public IIndexBuffer
{
    JAM_OBJECT
public:
    CIndexBufferVulkan(CRendererVulkanPtr renderer);
    virtual ~CIndexBufferVulkan();
    
    virtual void Initialize(DataTypes dataType) override;
    virtual void Shutdown() override;
    virtual bool IsValid() const override;
    
    virtual uint64_t SizeRaw() const override;
    virtual void ResizeRaw(uint64_t newSize) override;
    
    virtual uint64_t ElementSize() const override;
    
    virtual void* LockRaw() override;
    virtual SIndexStream& Lock() override;
    virtual bool IsLocked() const override;
    virtual void Unlock(bool isNeedCommit = false) override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
protected:
    virtual void ElementSize(uint64_t elementSize) override;
    
private:
    uint64_t m_ElementSize;
    SIndexStream m_Stream;
    uint64_t m_Size;
    
    CRendererVulkanWeak m_Renderer;
    VkBuffer m_Buffer;
    VkDeviceMemory m_DeviceMemory;
    void* m_MappedData;
};

}; // namespace jam

#endif /* CINDEXBUFFERVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
