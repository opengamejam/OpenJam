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
        TextureCoors,
        Normal,
        Color,
        Tangent
    };
    
    struct SVertexStream
    {
        IVertexBufferWeak vertexBuffer;
        unsigned char offset;
        unsigned char stride;
        DataTypes dataType;
        int binding;
        
        SVertexStream(IVertexBufferWeak _vertexBuffer)
        : vertexBuffer(_vertexBuffer)
        {
        
        }
        
        SVertexStream()
        : vertexBuffer(IVertexBufferWeak())
        {
        
        }
        
        INL bool IsActive() const
        {
            return (dataType != Unknown &&
                    stride != 0);
        }
        
        template <class T>
        INL void Set(size_t startIndex, const std::vector<T>& array)
        {
            IVertexBufferPtr vb = vertexBuffer.lock();
            if (!vb && IsActive())
            {
                return;
            }
            
            bool needRestoreLock = !vb->IsLocked();
            char* dst = static_cast<char*>(vb->LockRaw());
            const char* src = reinterpret_cast<const char*>(array.data());
            
            size_t srcElemSize = sizeof(T);
            size_t dstElemSize = sizeForType(dataType);
            size_t sizeElemToCopy = dstElemSize * stride;
            
            size_t srcSize = array.size() * srcElemSize;
            size_t dstSize = (vb->Size() - startIndex) * dstElemSize * stride;
            size_t sizeToCopy = std::min(srcSize, dstSize);
            
            size_t srcIndex = 0;
            size_t dstIndex = 0;
            size_t num = sizeToCopy / (dstElemSize * stride);
            for (size_t i = 0; i < num; ++i)
            {
                dstIndex = (startIndex + i) * vb->ElementSize() + offset;
                srcIndex = i * sizeElemToCopy;
                
                memcpy(&dst[dstIndex], &src[srcIndex], sizeElemToCopy);
            }
            
            if (needRestoreLock)
            {
                vb->Unlock();
            }
        }
        
        size_t sizeForType(DataTypes type)
        {
            static std::map<DataTypes, size_t> types = {
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
    
    typedef std::map<VertexTypes, SVertexStream> TVertexStreamMap;
    
public:
    IVertexBuffer() = default;
    virtual ~IVertexBuffer() = default;
    
    virtual void Initialize(size_t elementSize) = 0;
    virtual void Destroy() = 0;
    virtual bool IsValid() const = 0;
    
    virtual size_t ElementSize() const = 0;
    
    virtual size_t SizeRaw() const = 0;
    size_t Size()
    {
        if (SizeRaw() % ElementSize() == 0)
        {
            return (SizeRaw() / ElementSize());
        }
        return 0;
    }
    
    virtual void ResizeRaw(size_t newSize) = 0;
    void Resize(size_t newSize)
    {
        ResizeRaw(newSize * ElementSize());
    }
    
    virtual void* LockRaw() = 0;
    virtual SVertexStream& Lock(VertexTypes vertexType) = 0;
    virtual bool IsLocked() const = 0;
    virtual void Unlock() = 0;
    virtual bool HasStream(VertexTypes vertexType) = 0;
    
    virtual const TVertexStreamMap& VertexStreams() const = 0;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
protected:
    virtual void ElementSize(size_t elementSize) = 0;
};
    
}; // namespace jam;

#endif /* IVERTEXBUFFER_H */
