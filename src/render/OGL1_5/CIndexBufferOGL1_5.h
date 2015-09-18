//
//  CIndexBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CINDEXBUFFEROGL1_5_H
#define CINDEXBUFFEROGL1_5_H

#include "IIndexBuffer.h"

namespace jam
{
    
class CIndexBufferOGL1_5 : public IIndexBuffer
{
public:
    CIndexBufferOGL1_5();
    virtual ~CIndexBufferOGL1_5();
    
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

#endif /* defined(CINDEXBUFFEROGL1_5_H) || defined(RENDER_OGLES1_1) */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */