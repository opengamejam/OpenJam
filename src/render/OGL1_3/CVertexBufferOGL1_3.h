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
    
    virtual const TVertexStreamMap& VertexStreams() const;
    virtual SVertexStream& Lock(VertexTypes vertexType);
    
    virtual void Initialize(size_t elementSize);
    virtual void Shutdown();
    virtual bool IsValid() const;
    
    virtual size_t SizeRaw() const;
    virtual void ResizeRaw(size_t newSize);
    
    virtual size_t ElementSize() const;
    
    virtual void* LockRaw();
    virtual bool IsLocked() const;
    virtual void Unlock(bool isNeedCommit = false);
    virtual bool HasStream(VertexTypes vertexType);
    
    virtual void ZeroStride(bool isZeroStride);
    virtual bool ZeroStride();
    
    virtual void Bind();
    virtual void Unbind();
    
protected:
    virtual void ElementSize(size_t elementSize);
    
private:
    int ConvertDataType(DataTypes dataType);
    
private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    size_t m_ElementSize;
    bool m_IsLocked;
    TVertexStreamMap m_VertexStreamers;
    bool m_ZeroStride;
};
    
}; // namespace jam

#endif /* defined(CVERTEXBUFFEROGL1_3_H) */

#endif /* RENDER_OGL1_3 */