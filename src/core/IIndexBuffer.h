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
    
CLASS_PTR(IIndexBuffer)
    
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
        
        uint64_t size = (ib->Size() - startIndex < srcData.size() ? ib->Size() - startIndex : srcData.size());
        for (uint64_t i = 0; i < size; ++i)
        {
            T* dst = Map<T>(ib, startIndex + i);
            const T* src = srcData.data() + i;
            memcpy(dst, src, sizeof(T));
        }
        
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
        
        T* dst = Map<T>(ib, startIndex);
        const T* src = &srcData;
        memcpy(dst, src, sizeof(T));
        
        if (needRestoreLock)
        {
            ib->Unlock();
        }
    }
    
    template <class T>
    INL void SetUnsafe(uint64_t startIndex, const T& srcData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!ib)
        {
            return;
        }
        
        T* dst = Map<T>(ib, startIndex);
        *dst = srcData;
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
        
        bool needRestoreLock = !ib->IsLocked();
        
        uint64_t size = (ib->Size() - startIndex < dstData.size() ? ib->Size() - startIndex : dstData.size());
        for (uint64_t i = 0; i < size; ++i)
        {
            T* dst = dstData.data() + i;
            const T* src = Map<const T>(ib, startIndex + i);
            memcpy(dst, src, sizeof(T));
        } 
        
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
        
        T* dst = &dstData;
        const T* src = Map<const T>(ib, startIndex);
        memcpy(dst, src, sizeof(T));
        
        if (needRestoreLock)
        {
            ib->Unlock();
        }
        
        return true;
    }
    
    template <class T>
    INL void GetUnsafe(uint64_t startIndex, T& dstData)
    {
        IIndexBufferPtr ib = indexBuffer.lock();
        if (!ib)
        {
            return;
        }
        
        const T* src = Map<const T>(ib, startIndex);
        dstData = *src;
    }
    
    INL uint64_t DataSize() const
    {
        return SizeForType(dataType);
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
    INL T* Map(IIndexBufferPtr ib, uint64_t startIndex)
    {
        assert(ib);
        uint64_t elemSizeToCopy = DataSize();
        
        int8_t* src = static_cast<int8_t*>(ib->LockRaw());
        return reinterpret_cast<T*>(src + startIndex * elemSizeToCopy);
    }
};
    
}; // namespace jam;

#endif /* IINDEXBUFFER_H */
