//
//  CVertexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CVERTEXBUFFEROGL2_0_H
#define CVERTEXBUFFEROGL2_0_H

#include "IVertexBuffer.h"

namespace jam
{
    
class CVertexBufferOGL2_0 : public IVertexBuffer
{
public:
    CVertexBufferOGL2_0();
    virtual ~CVertexBufferOGL2_0();
    
    virtual const TVertexStreamMap& VertexStreams() const;
    virtual SVertexStream& Lock(VertexTypes vertexType);
    
    virtual void Initialize(uint64_t elementSize);
    virtual void Destroy();
    virtual bool IsValid() const;
    
    virtual uint64_t SizeRaw() const;
    virtual void ResizeRaw(uint64_t newSize);
    
    virtual uint64_t ElementSize() const;
    
    virtual void* LockRaw();
    virtual bool IsLocked() const;
    virtual void Unlock();
    virtual bool HasStream(VertexTypes vertexType);
    
    virtual void ZeroStride(bool isZeroStride);
    virtual bool ZeroStride();
    
    virtual void Bind();
    virtual void Unbind();
    
protected:
    virtual void ElementSize(uint64_t elementSize);
    
private:
    int ConvertDataType(DataTypes dataType);
    
private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    uint64_t m_ElementSize;
    bool m_IsLocked;
    TVertexStreamMap m_VertexStreamers;
    bool m_ZeroStride;
};
    
}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL2_0_H) */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */