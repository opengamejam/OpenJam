//
//  CIndexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CIndexBufferOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGL2_0::CIndexBufferOGL2_0()
: m_Id(0)
, m_ElementSize(1)
{
    
}

CIndexBufferOGL2_0::~CIndexBufferOGL2_0()
{
    Destroy();
}

void CIndexBufferOGL2_0::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

void CIndexBufferOGL2_0::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGL2_0::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGL2_0::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL2_0::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGL2_0::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL2_0::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGL2_0::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
}

void CIndexBufferOGL2_0::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGL2_0::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGL2_0::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0