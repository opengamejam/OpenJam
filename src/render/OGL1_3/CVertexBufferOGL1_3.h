//
//  CVertexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CVERTEXBUFFEROGL1_3_H
#define CVERTEXBUFFEROGL1_3_H

#include "IVertexBuffer.h"

namespace jam
{
    
class CVertexBufferOGL1_3 : public IVertexBuffer
{
public:
    CVertexBufferOGL1_3();
    virtual ~CVertexBufferOGL1_3();
    
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
    virtual void Unlock() override;
    virtual bool HasStream(VertexTypes vertexType) override;
    
    virtual void ZeroStride(bool isZeroStride) override;
    virtual bool ZeroStride() override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
protected:
    virtual void ElementSize(uint64_t elementSize) override;
    
private:
    int ConvertDataType(DataTypes dataType);
    
private:
    uint32_t m_Id;
    std::vector<uint8_t> m_Buffer;
    uint64_t m_ElementSize;
    bool m_IsLocked;
    TVertexStreamMap m_VertexStreamers;
    bool m_ZeroStride;
};
    
}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL1_3_H) */

#endif /* RENDER_OGL1_3 */