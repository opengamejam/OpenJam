//
//  IGameObject.h
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
    
class IIndexBuffer
{
public:
    IIndexBuffer() = default;
    virtual ~IIndexBuffer() = default;
    
    virtual void Initialize(size_t elementSize) = 0;
    virtual void Destroy() = 0;
    virtual bool IsValid() const = 0;
    
    virtual size_t SizeRaw() const = 0;
    size_t Size()
    {
        return (SizeRaw() / ElementSize());
    }
    
    virtual void ResizeRaw(size_t newSize) = 0;
    
    void Resize(size_t newSize)
    {
        ResizeRaw(newSize * ElementSize());
    }
    
    virtual size_t ElementSize() const = 0;
    
    virtual void* LockRaw() = 0;
    
    template<class T>
    T* Lock()
    {
        if (sizeof(T) == ElementSize())
        {
            T* buffer = static_cast<T*>(LockRaw());
            return buffer;
        }
        return nullptr;
    }
    
    virtual void Unlock() = 0;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
protected:
    virtual void ElementSize(size_t elementSize) = 0;
};
    
}; // namespace jam;

#endif /* IINDEXBUFFER_H */
