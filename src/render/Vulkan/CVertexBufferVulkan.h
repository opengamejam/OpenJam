//
//  CVertexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CVERTEXBUFFERVULKAN_H
#define CVERTEXBUFFERVULKAN_H

#include "IVertexBuffer.h"

namespace jam {

class CVertexBufferVulkan : public IVertexBuffer
{
    JAM_OBJECT
public:
    CVertexBufferVulkan(const VkDevice& logicalDevice);
    virtual ~CVertexBufferVulkan();
    
    virtual const TVertexStreamMap& VertexStreams() const override;
    virtual SVertexStream& Lock(VertexTypes vertexType) override;
    
    virtual void Initialize(uint64_t elementSize) override;
    virtual void Shutdown() override;
    virtual bool IsValid() const override;
    
    virtual uint64_t SizeRaw() const override;
    virtual void ResizeRaw(uint64_t newSize) override;
    
    virtual uint64_t ElementSize() const override;
    
    virtual void* LockRaw() override;
    virtual bool IsLocked() const override;
    virtual void Unlock(bool isNeedCommit = false) override;
    virtual bool HasStream(VertexTypes vertexType) override;
    
    virtual void ZeroStride(bool isZeroStride) override;
    virtual bool ZeroStride() override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
protected:
    virtual void ElementSize(uint64_t elementSize) override;
    
private:
    uint64_t m_ElementSize;
    TVertexStreamMap m_VertexStreamers;
    bool m_ZeroStride;
    uint64_t m_Size;
    
    VkDevice m_LogicalDevice;
    VkBuffer m_Buffer;
    VkDeviceMemory m_DeviceMemory;
    void* m_MappedData;
};

}; // namespace jam

#endif /* defined(CVERTEXBUFFERVULKAN_H) */

//#endif /* defined(RENDER_VULKAN) */
