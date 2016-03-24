//
//  ISprite.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IVERTEXBUFFER_H
#define IVERTEXBUFFER_H

#include "Global.h"

namespace jam
{
    
CLASS_PTR(IVertexBuffer);
CLASS_WEAK(IVertexBuffer);
    
class IVertexBuffer : public std::enable_shared_from_this<IVertexBuffer>
{
public:
    enum DataTypes
    {
        Unknown = -1,
        Byte = 0,
        UByte,
        Short,
        UShort,
        Int,
        UInt,
        Float,
        ShortFloat,
    };
    
    enum VertexTypes
    {
        Position,
        TextureCoords,
        Normal,
        Color,
        Tangent
    };
    
    struct SVertexStream;
    typedef std::map<VertexTypes, SVertexStream> TVertexStreamMap;
    
public:
    IVertexBuffer() = default;
    virtual ~IVertexBuffer() = default;
    
    virtual void Initialize(uint64_t elementSize) = 0;
    virtual void Shutdown() = 0;
    virtual bool IsValid() const = 0;
    
    virtual uint64_t ElementSize() const = 0;
    
    virtual uint64_t SizeRaw() const = 0;
    uint64_t Size()
    {
        if (SizeRaw() % ElementSize() == 0)
        {
            return (SizeRaw() / ElementSize());
        }
        return 0;
    }
    
    virtual void ResizeRaw(uint64_t newSize) = 0;
    void Resize(uint64_t newSize)
    {
        ResizeRaw(newSize * ElementSize());
    }
    
    virtual void* LockRaw() = 0;
    virtual SVertexStream& Lock(VertexTypes vertexType) = 0;
    virtual bool IsLocked() const = 0;
    virtual void Unlock(bool isNeedCommit = false) = 0;
    virtual bool HasStream(VertexTypes vertexType) = 0;
    
    virtual void ZeroStride(bool isZeroStride) = 0;
    virtual bool ZeroStride() = 0;
    
    virtual const TVertexStreamMap& VertexStreams() const = 0;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
protected:
    virtual void ElementSize(uint64_t elementSize) = 0;
};
    
    
/*
 
 Vertex streams allow to work with discrete elements as one whole
 
 */
struct IVertexBuffer::SVertexStream
{
    IVertexBufferWeak vertexBuffer;
    uint8_t offset;
    uint8_t stride;
    DataTypes dataType;
    int32_t attributeIndex;
    uint32_t streamIndex;
    uint64_t absoluteOffset; // Used for zero-stride mode
    
    SVertexStream(IVertexBufferWeak _vertexBuffer)
    : vertexBuffer(_vertexBuffer)
    , offset(0)
    , stride(0)
    , dataType(Unknown)
    , attributeIndex(0)
    , streamIndex(0)
    , absoluteOffset(0)
    {
        
    }
    
    SVertexStream()
    : vertexBuffer(IVertexBufferWeak())
    , offset(0)
    , stride(0)
    , dataType(Unknown)
    , attributeIndex(0)
    , streamIndex(0)
    , absoluteOffset(0)
    {
        
    }
    
    INL bool IsActive() const
    {
        return (dataType != Unknown &&
                stride != 0);
    }
    
    void CopyFrom(const SVertexStream& srcStream, uint64_t srcIndex, uint64_t size, uint64_t dstIndex)
    {
        IVertexBufferPtr dstVB = vertexBuffer.lock();
        IVertexBufferPtr srcVB = srcStream.vertexBuffer.lock();
        if (!IsActive() || !srcStream.IsActive() ||
            !dstVB || !srcVB ||
            dstIndex + size >= dstVB->Size() ||
            srcIndex + size >= srcVB->Size())
        {
            return;
        }
        
        bool needRestoreLockSourceVB = !srcVB->IsLocked();
        bool needRestoreLockDestinationVB = !dstVB->IsLocked();
        
        for (uint64_t i = 0; i < size; ++i)
        {
            uint8_t* dst = Map<uint8_t>(dstVB, dstIndex + i);
            const uint8_t* src = Map<const uint8_t>(srcVB, srcIndex + i);
            memcpy(dst, src, srcStream.DataSize());
        }
        
        if (needRestoreLockSourceVB)
        {
            srcVB->Unlock();
        }
        if (needRestoreLockDestinationVB)
        {
            dstVB->Unlock();
        }
    }
    
    template <class T>
    INL void Set(uint64_t startIndex, const std::vector<T>& srcData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!IsActive() ||
            !vb ||
            startIndex >= vb->Size())
        {
            return;
        }
        
        bool needRestoreLock = !vb->IsLocked();
        
        uint64_t size = (vb->Size() - startIndex < srcData.size() ? vb->Size() - startIndex : srcData.size());
        for (uint64_t i = 0; i < size; ++i)
        {
            T* dst = Map<T>(vb, startIndex + i);
            const T* src = srcData.data() + i;
            memcpy(dst, src, sizeof(T));
        }
        
        if (needRestoreLock)
        {
            vb->Unlock();
        }
    }
    
    template <class T>
    INL void Set(uint64_t startIndex, const T& srcData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!IsActive() ||
            !vb ||
            startIndex >= vb->Size())
        {
            return;
        }
        
        bool needRestoreLock = !vb->IsLocked();
        
        T* dst = Map<T>(vb, startIndex);
        const T* src = &srcData;
        memcpy(dst, src, sizeof(T));
        
        if (needRestoreLock)
        {
            vb->Unlock();
        }
    }
    
    template <class T>
    INL void SetUnsafe(uint64_t startIndex, const T& srcData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!vb)
        {
            return;
        }
        
        T* dst = Map<T>(vb, startIndex);
        *dst = srcData;
    }
    
    template <class T>
    INL bool Get(uint64_t startIndex, std::vector<T>& dstData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!IsActive() ||
            !vb ||
            startIndex >= vb->Size())
        {
            return false;
        }
        
        bool needRestoreLock = !vb->IsLocked();
        
        uint64_t size = (vb->Size() - startIndex < dstData.size() ? vb->Size() - startIndex : dstData.size());
        for (uint64_t i = 0; i < size; ++i)
        {
            T* dst = dstData.data() + i;
            const T* src = Map<const T>(vb, startIndex + i);
            memcpy(dst, src, sizeof(T));
        }
        
        if (needRestoreLock)
        {
            vb->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL bool Get(uint64_t startIndex, T& dstData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!IsActive() ||
            !vb ||
            startIndex >= vb->Size())
        {
            return false;
        }
        
        bool needRestoreLock = !vb->IsLocked();
        
        T* dst = &dstData;
        const T* src = Map<const T>(vb, startIndex);
        memcpy(dst, src, sizeof(T));
        
        if (needRestoreLock)
        {
            vb->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL void GetUnsafe(uint64_t startIndex, T& dstData)
    {
        IVertexBufferPtr vb = vertexBuffer.lock();
        if (!vb)
        {
            return;
        }
        
        const T* src = Map<const T>(vb, startIndex);
        dstData = *src;
    }
    
    INL uint64_t DataSize() const
    {
        return SizeForType(dataType) * stride;
    }
    
    INL static uint32_t SizeForType(DataTypes type)
    {
        static std::map<DataTypes, uint32_t> types =
        {
            {Unknown, 0},
            {Byte, sizeof(char)},
            {UByte, sizeof(unsigned char)},
            {Short, sizeof(short)},
            {UShort, sizeof(unsigned short)},
            {Int, sizeof(int)},
            {UInt, sizeof(unsigned int)},
            {Float, sizeof(float)},
            {ShortFloat, 2}, // TODO:
        };
        
        return types[type];
    }
    
private:
    template <class T>
    INL T* Map(IVertexBufferPtr vb, uint64_t startIndex)
    {
        assert(vb);
        uint64_t elemSizeToCopy = DataSize();
        
        uint8_t* src = static_cast<uint8_t*>(vb->LockRaw());
        if (vb->ZeroStride())
        {
            return reinterpret_cast<T*>(src + absoluteOffset + startIndex * elemSizeToCopy);
        }
        else
        {
            return reinterpret_cast<T*>(src + startIndex * vb->ElementSize() + offset);
        }
    }
};

INL bool CompareCapability(IVertexBufferPtr vb1, IVertexBufferPtr vb2)
{
    if (!vb1 || !vb2)
    {
        return false;
    }
    
    const IVertexBuffer::TVertexStreamMap& stream1 = vb1->VertexStreams();
    const IVertexBuffer::TVertexStreamMap& stream2 = vb2->VertexStreams();
    
    auto pred = [](const IVertexBuffer::TVertexStreamMap::value_type& s1,
                   const IVertexBuffer::TVertexStreamMap::value_type& s2) -> bool
    {
        return s1.first == s2.first;
    };
    
    return (stream1.size() == stream2.size() &&
            std::equal(stream1.begin(), stream1.end(), stream2.begin(), pred));
}
    
}; // namespace jam;

#endif /* IVERTEXBUFFER_H */
