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
, m_IsLocked(false)
{
    
}

CIndexBufferOGL2_0::~CIndexBufferOGL2_0()
{
    Destroy();
}

void CIndexBufferOGL2_0::Initialize(uint64_t elementSize)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
        m_Stream = IIndexBuffer::SIndexStream(shared_from_this());
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

uint64_t CIndexBufferOGL2_0::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL2_0::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize(newSize);
}

uint64_t CIndexBufferOGL2_0::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL2_0::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

IIndexBuffer::SIndexStream& CIndexBufferOGL2_0::Lock()
{
    return m_Stream;
}

bool CIndexBufferOGL2_0::IsLocked() const
{
    return m_IsLocked;
}

void CIndexBufferOGL2_0::Unlock()
{    
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
    m_IsLocked = false;
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

void CIndexBufferOGL2_0::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0