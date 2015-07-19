//
//  CIndexBufferOGLES1.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 4/14/15.
//
//
#if defined(RENDER_OGLES1)

#include "CIndexBufferOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGLES1::CIndexBufferOGLES1()
: m_Id(0)
, m_ElementSize(1)
{
    
}

CIndexBufferOGLES1::~CIndexBufferOGLES1()
{
    Destroy();
}

void CIndexBufferOGLES1::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

void CIndexBufferOGLES1::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGLES1::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGLES1::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGLES1::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGLES1::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGLES1::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGLES1::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
}

void CIndexBufferOGLES1::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGLES1::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGLES1::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES1