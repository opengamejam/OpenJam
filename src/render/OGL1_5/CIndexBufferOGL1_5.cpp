//
//  CIndexBufferOGL1_5.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 4/14/15.
//
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CIndexBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGL1_5::CIndexBufferOGL1_5()
: m_Id(0)
, m_ElementSize(1)
{
    
}

CIndexBufferOGL1_5::~CIndexBufferOGL1_5()
{
    Destroy();
}

void CIndexBufferOGL1_5::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
    }
    ElementSize(elementSize);
}

void CIndexBufferOGL1_5::Destroy()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGL1_5::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGL1_5::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL1_5::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGL1_5::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL1_5::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGL1_5::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
}

void CIndexBufferOGL1_5::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGL1_5::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGL1_5::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1