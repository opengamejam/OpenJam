//
//  CIndexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CINDEXBUFFEROGL1_3_H
#define CINDEXBUFFEROGL1_3_H

#include "IIndexBuffer.h"

namespace jam
{
    
class CIndexBufferOGL1_3 : public IIndexBuffer
{
public:
    CIndexBufferOGL1_3();
    virtual ~CIndexBufferOGL1_3();
    
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

#endif /* defined(CINDEXBUFFEROGL1_3_H) */

#endif /* RENDER_OGL1_3 */