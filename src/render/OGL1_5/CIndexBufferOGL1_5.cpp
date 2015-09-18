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
, m_IsLocked(false)
{
    
}

CIndexBufferOGL1_5::~CIndexBufferOGL1_5()
{
    Destroy();
}

void CIndexBufferOGL1_5::Initialize(uint64_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
        m_Stream = IIndexBuffer::SIndexStream(shared_from_this());
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

uint64_t CIndexBufferOGL1_5::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL1_5::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize(newSize);
}

uint64_t CIndexBufferOGL1_5::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL1_5::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

IIndexBuffer::SIndexStream& CIndexBufferOGL1_5::Lock()
{
    return m_Stream;
}

bool CIndexBufferOGL1_5::IsLocked() const
{
    return m_IsLocked;
}

void CIndexBufferOGL1_5::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
    m_IsLocked = false;
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

void CIndexBufferOGL1_5::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1