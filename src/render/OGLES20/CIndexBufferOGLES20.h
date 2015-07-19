//
//  CIndexBufferOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#ifndef CINDEXBUFFEROGLES20_H
#define CINDEXBUFFEROGLES20_H

#include "IIndexBuffer.h"

namespace jam
{
    
class CIndexBufferOGLES20 : public IIndexBuffer
{
public:
    CIndexBufferOGLES20();
    virtual ~CIndexBufferOGLES20();
    
    virtual void Initialize(size_t elementSize);
    virtual void Destroy();
    virtual bool IsValid() const;
    
    virtual size_t SizeRaw() const;
    virtual void ResizeRaw(size_t newSize);
    
    virtual size_t ElementSize() const;
    
    virtual void* LockRaw();
    virtual void Unlock();
    
    virtual void Bind();
    virtual void Unbind();
    
protected:
    virtual void ElementSize(size_t elementSize);
    
private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    size_t m_ElementSize;
};
    
}; // namespace jam

#endif /* defined(CINDEXBUFFEROGLES20_H) */

#endif /* RENDER_OGLES2 */