//
//  CVertexBufferOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#ifndef CVERTEXBUFFEROGLES1_H
#define CVERTEXBUFFEROGLES1_H

#include "IVertexBuffer.h"

namespace jam
{
    
class CVertexBufferOGLES1 : public IVertexBuffer
{
public:
    CVertexBufferOGLES1();
    virtual ~CVertexBufferOGLES1();
    
    virtual const TVertexStreamMap& VertexStreams() const;
    virtual SVertexStream& Lock(VertexTypes vertexType);
    
    virtual void Initialize(size_t elementSize);
    virtual void Destroy();
    virtual bool IsValid() const;
    
    virtual size_t SizeRaw() const;
    virtual void ResizeRaw(size_t newSize);
    
    virtual size_t ElementSize() const;
    
    virtual void* LockRaw();
    virtual bool IsLocked() const;
    virtual void Unlock();
    virtual bool HasStream(VertexTypes vertexType);
    
    virtual void Bind();
    virtual void Unbind();
    
protected:
    virtual void ElementSize(size_t elementSize);
    
private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    size_t m_ElementSize;
    bool m_IsLocked;
    TVertexStreamMap m_VertexStreamers;
};
    
}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGLES1_H) */

#endif /* RENDER_OGLES1 */