//
//  CIndexBufferOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CIndexBufferOGLES2.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGLES2::CIndexBufferOGLES2()
: m_Id(0)
, m_ElementSize(1)
{
    
}

CIndexBufferOGLES2::~CIndexBufferOGLES2()
{
    Destroy();
}

void CIndexBufferOGLES2::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

void CIndexBufferOGLES2::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGLES2::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGLES2::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGLES2::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGLES2::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGLES2::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGLES2::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
}

void CIndexBufferOGLES2::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGLES2::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGLES2::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2