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

#endif /* defined(CINDEXBUFFEROGL2_0_H) */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */