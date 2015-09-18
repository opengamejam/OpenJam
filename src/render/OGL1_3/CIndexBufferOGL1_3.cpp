//
//  CIndexBufferOGL1_3.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 4/14/15.
//
//
#if defined(RENDER_OGL1_3)

#include "CIndexBufferOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGL1_3::CIndexBufferOGL1_3()
: m_Id(0)
, m_ElementSize(1)
, m_IsLocked(false)
{
    
}

CIndexBufferOGL1_3::~CIndexBufferOGL1_3()
{
    Shutdown();
}

void CIndexBufferOGL1_3::Initialize(uint64_t elementSize)
{
    if (!IsValid())
    {
#ifdef GL_ELEMENT_ARRAY_BUFFER
        glGenBuffers(1, &m_Id);
#else
        m_Id = 1;
#endif
        m_Stream = IIndexBuffer::SIndexStream(shared_from_this());
    }
    ElementSize(elementSize);
}

void CIndexBufferOGL1_3::Shutdown()
{
    if (IsValid())
    {
#ifdef GL_ELEMENT_ARRAY_BUFFER
        glDeleteBuffers(1, &m_Id);
#endif
        m_Id = 0;
    }
}

bool CIndexBufferOGL1_3::IsValid() const
{
    return (m_Id != 0);
}

uint64_t CIndexBufferOGL1_3::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL1_3::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize(newSize);
}

uint64_t CIndexBufferOGL1_3::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL1_3::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

IIndexBuffer::SIndexStream& CIndexBufferOGL1_3::Lock()
{
    return m_Stream;
}

bool CIndexBufferOGL1_3::IsLocked() const
{
    return m_IsLocked;
}

void CIndexBufferOGL1_3::Unlock()
{
#ifdef GL_ELEMENT_ARRAY_BUFFER
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
    m_IsLocked = false;
#endif
}

void CIndexBufferOGL1_3::Bind()
{
#ifdef GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
#endif
}

void CIndexBufferOGL1_3::Unbind()
{
#ifdef GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGL1_3::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3