//
//  CVertexBufferOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CVERTEXBUFFEROGLBASE_H
#define CVERTEXBUFFEROGLBASE_H

#include "IVertexBuffer.h"

namespace jam
{
    
class CVertexBufferOGLBase : public IVertexBuffer
{
public:
    CVertexBufferOGLBase();
    virtual ~CVertexBufferOGLBase();
    
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
    virtual void Unlock(bool isNeedCommit = false) override;
    virtual bool HasStream(VertexTypes vertexType) override;
    
    virtual void ZeroStride(bool isZeroStride) override;
    virtual bool ZeroStride() override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
    /*
     * OpenGL specific
     */
    GLenum ConvertDataType(DataTypes dataType);
    
protected:
    virtual void ElementSize(uint64_t elementSize) override;
    
private:
    uint32_t m_Id;
    std::vector<uint8_t> m_Buffer;
    uint64_t m_ElementSize;
    bool m_IsLocked;
    TVertexStreamMap m_VertexStreamers;
    bool m_ZeroStride;
};
    
}; // namespace jam

#endif /* CVERTEXBUFFEROGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
