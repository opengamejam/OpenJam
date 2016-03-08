//
//  IIndexBuffer.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IINDEXBUFFER_H
#define IINDEXBUFFER_H

#include "Global.h"

namespace jam
{
    
CLASS_PTR(IIndexBuffer);
CLASS_WEAK(IIndexBuffer);
    
class IIndexBuffer : public std::enable_shared_from_this<IIndexBuffer>
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
    
    struct SIndexStream;
    
public:
    IIndexBuffer() = default;
    virtual ~IIndexBuffer() = default;
    
    virtual void Initialize(DataTypes dataType) = 0;
    virtual void Shutdown() = 0;
    virtual bool IsValid() const = 0;
    
    virtual uint64_t SizeRaw() const = 0;
    uint64_t Size()
    {
        return (SizeRaw() / ElementSize());
    }
    
    virtual void ResizeRaw(uint64_t newSize) = 0;
    
    void Resize(uint64_t newSize)
    {
        ResizeRaw(newSize * ElementSize());
    }
    
    virtual uint64_t ElementSize() const = 0;
    
    virtual void* LockRaw() = 0;
    virtual SIndexStream& Lock() = 0;
    virtual bool IsLocked() const = 0;
    virtual void Unlock(bool isNeedCommit = false) = 0;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
protected:
    virtual void ElementSize(uint64_t elementSize) = 0;
};
    

    
    
struct IIndexBuffer::SIndexStream
{
    IIndexBufferWeak indexBuffer;
    DataTypes dataType;
    
    SIndexStream(IIndexBufferWeak _indexBuffer)
    : indexBuffer(_indexBuffer)
    , dataType(Unknown)
    {
        
    }
    
    SIndexStream()
    : indexBuffer(IIndexBufferWeak())
    , dataType(Unknown)
    {
        
    }
    
    INL bool IsActive() const
    {
        return (dataType != Unknown);
    }
    
    template <class T>
    INL void Set(uint64_t startIndex, const std::vector<T>& srcData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!IsActive() ||
            !ib ||
            startIndex >= ib->Size())
        {
            return;
        }
        
        bool needRestoreLock = !ib->IsLocked();
        char* dst = static_cast<char*>(ib->LockRaw());
        const char* src = reinterpret_cast<const char*>(srcData.data());
        
        uint32_t dataSize = sizeForType(dataType);
        uint64_t srcSize = srcData.size() * sizeof(T);
        uint64_t dstSize = (ib->Size() - startIndex) * dataSize;
        uint64_t sizeToCopy = std::min(srcSize, dstSize);
        
        memcpy(dst + startIndex * dataSize, src, sizeToCopy);
        
        if (needRestoreLock)
        {
            ib->Unlock();
        }
    }
    
    template <class T>
    INL void Set(uint64_t startIndex, const T& srcData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!IsActive() ||
            !ib ||
            startIndex >= ib->Size())
        {
            return;
        }

        bool needRestoreLock = !ib->IsLocked();
        SetUnsafe<T>(ib, startIndex, srcData);
        if (needRestoreLock)
        {
            ib->Unlock();
        }
    }
    
    template <class T>
    INL void SetUnsafe(IIndexBufferPtr ib, uint64_t startIndex, const T& srcData)
    {
        uint32_t dataSize = sizeForType(dataType);
        uint64_t sizeToCopy = sizeof(T);
        
        char* dst = static_cast<char*>(ib->LockRaw());
        const char* src = reinterpret_cast<const char*>(&srcData);
        
        memcpy(dst + startIndex * dataSize, src, sizeToCopy);
    }
    
    template <class T>
    INL bool Get(uint64_t startIndex, std::vector<T>& dstData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!IsActive() ||
            !ib ||
            startIndex >= ib->Size())
        {
            return false;
        }
        
        dstData.resize(ib->Size() - startIndex);
        uint32_t dataSize = sizeForType(dataType);
        uint64_t sizeToCopy = (ib->Size() - startIndex) * dataSize;
        
        bool needRestoreLock = !ib->IsLocked();
        char* dst = reinterpret_cast<char*>(dstData.data());
        const char* src = static_cast<const char*>(ib->LockRaw());
        
        memcpy(dst, src + startIndex * dataSize, sizeToCopy);
        
        if (needRestoreLock)
        {
            ib->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL bool Get(uint64_t startIndex, T& dstData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!IsActive() ||
            !ib ||
            startIndex >= ib->Size())
        {
            return false;
        }
        
        bool needRestoreLock = !ib->IsLocked();
        GetUnsafe<T>(ib, startIndex, dstData);
        if (needRestoreLock)
        {
            ib->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL void GetUnsafe(IIndexBufferPtr ib, uint64_t startIndex, T& dstData)
    {
        uint32_t dataSize = sizeForType(dataType);
        uint64_t sizeToCopy = sizeof(T);
        
        char* dst = reinterpret_cast<char*>(&dstData);
        const char* src = static_cast<const char*>(ib->LockRaw());
        
        memcpy(dst, src + startIndex * dataSize, sizeToCopy);
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
    
}; // namespace jam;

#endif /* IINDEXBUFFER_H */
