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
        char* dst = static_cast<char*>(vb->LockRaw());
        const char* src = reinterpret_cast<const char*>(srcData.data());
        
        uint32_t dataSize = sizeForType(dataType);
        uint64_t elemSizeToCopy = dataSize * stride;
        uint64_t srcSize = srcData.size() * sizeof(T);
        uint64_t dstSize = (vb->Size() - startIndex) * elemSizeToCopy;
        uint64_t sizeToCopy = std::min(srcSize, dstSize);
        
        if (vb->ZeroStride())
        {
            memcpy(dst + absoluteOffset + startIndex * elemSizeToCopy, src, sizeToCopy);
        }
        else
        {
            uint64_t srcIndex = 0;
            uint64_t dstIndex = 0;
            uint64_t num = sizeToCopy / elemSizeToCopy;
            for (uint64_t i = 0; i < num; ++i)
            {
                dstIndex = (startIndex + i) * vb->ElementSize() + offset;
                srcIndex = i * elemSizeToCopy;
                
                memcpy(&dst[dstIndex], &src[srcIndex], elemSizeToCopy);
            }
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
        SetUnsafe<T>(vb, startIndex, srcData);
        if (needRestoreLock)
        {
            vb->Unlock();
        }
    }
    
    template <class T>
    INL void SetUnsafe(IVertexBufferPtr vb, uint64_t startIndex, const T& srcData)
    {
        uint32_t dataSize = sizeForType(dataType);
        uint64_t elemSizeToCopy = dataSize * stride;
        uint64_t sizeToCopy = sizeof(T);
        
        char* dst = static_cast<char*>(vb->LockRaw());
        const char* src = reinterpret_cast<const char*>(&srcData);
        
        if (vb->ZeroStride())
        {
            memcpy(dst + absoluteOffset + startIndex * elemSizeToCopy, src, sizeToCopy);
        }
        else
        {
            memcpy(dst + startIndex * vb->ElementSize() + offset, src, sizeToCopy);
        }
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
        
        dstData.resize(vb->Size() - startIndex);
        uint32_t dataSize = sizeForType(dataType);
        uint64_t elemSizeToCopy = dataSize * stride;
        uint64_t sizeToCopy = (vb->Size() - startIndex) * elemSizeToCopy;
        
        bool needRestoreLock = !vb->IsLocked();
        char* dst = reinterpret_cast<char*>(dstData.data());
        const char* src = static_cast<const char*>(vb->LockRaw());
        
        if (vb->ZeroStride())
        {
            memcpy(dst, src + absoluteOffset + startIndex * elemSizeToCopy, sizeToCopy);
        }
        else
        {
            uint64_t srcIndex = 0;
            uint64_t dstIndex = 0;
            uint64_t num = vb->Size() - startIndex;
            for (uint64_t i = 0; i < num; ++i)
            {
                dstIndex = i * elemSizeToCopy;
                srcIndex = (startIndex + i) * vb->ElementSize() + offset;
                
                memcpy(&dst[dstIndex], &src[srcIndex], elemSizeToCopy);
            }
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
        GetUnsafe<T>(vb, startIndex, dstData);
        if (needRestoreLock)
        {
            vb->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL void GetUnsafe(IVertexBufferPtr vb, uint64_t startIndex, T& dstData)
    {
        uint32_t dataSize = sizeForType(dataType);
        uint64_t elemSizeToCopy = dataSize * stride;
        uint64_t sizeToCopy = sizeof(T);
        
        char* dst = reinterpret_cast<char*>(&dstData);
        const char* src = static_cast<const char*>(vb->LockRaw());
        
        if (vb->ZeroStride())
        {
            memcpy(dst, src + absoluteOffset + startIndex * elemSizeToCopy, sizeToCopy);
        }
        else
        {
            memcpy(dst, src + startIndex * vb->ElementSize() + offset, sizeToCopy);
        }
    }
    
    uint64_t DataSize() const
    {
        return sizeForType(dataType);
    }
    
    static uint32_t sizeForType(DataTypes type)
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
};

bool CompareCapability(IVertexBufferPtr vb1, IVertexBufferPtr vb2)
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
