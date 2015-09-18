//
//  CIndexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CINDEXBUFFEROGL2_0_H
#define CINDEXBUFFEROGL2_0_H

#include "IIndexBuffer.h"

namespace jam
{
    
class CIndexBufferOGL2_0 : public IIndexBuffer
{
public:
    CIndexBufferOGL2_0();
    virtual ~CIndexBufferOGL2_0();
    
    virtual void Initialize(uint64_t elementSize);
    virtual void Destroy();
    virtual bool IsValid() const;
    
    virtual uint64_t SizeRaw() const;
    virtual void ResizeRaw(uint64_t newSize);
    
    virtual uint64_t ElementSize() const;
    
    virtual void* LockRaw();
    virtual SIndexStream& Lock();
    virtual bool IsLocked() const;
    virtual void Unlock();
    
    virtual void Bind();
    virtual void Unbind();
    
protected:
    virtual void ElementSize(uint64_t elementSize);
    
private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    uint64_t m_ElementSize;
    bool m_IsLocked;
    SIndexStream m_Stream;
};
    
}; // namespace jam

#endif /* defined(CINDEXBUFFEROGL2_0_H) */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */