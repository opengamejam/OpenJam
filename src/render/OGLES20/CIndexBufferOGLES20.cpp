//
//  CIndexBufferOGLES20.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CIndexBufferOGLES20.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGLES20::CIndexBufferOGLES20()
: m_Id(0)
, m_ElementSize(1)
{
    
}

CIndexBufferOGLES20::~CIndexBufferOGLES20()
{
    Destroy();
}

void CIndexBufferOGLES20::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

void CIndexBufferOGLES20::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGLES20::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGLES20::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGLES20::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGLES20::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGLES20::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGLES20::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
}

void CIndexBufferOGLES20::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGLES20::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGLES20::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2