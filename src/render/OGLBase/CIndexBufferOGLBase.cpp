//
//  CIndexBufferOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CIndexBufferOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGLBase::CIndexBufferOGLBase()
: m_Id(0)
, m_ElementSize(1)
, m_IsLocked(false)
{
    
}

CIndexBufferOGLBase::~CIndexBufferOGLBase()
{
    Shutdown();
}

void CIndexBufferOGLBase::Initialize(DataTypes dataType)
{
    if (!IsValid())
    {
        glGenBuffers(1, &m_Id);
        m_Stream = IIndexBuffer::SIndexStream(shared_from_this());
        m_Stream.dataType = dataType;
    }
    ElementSize(IIndexBuffer::SIndexStream::SizeForType(dataType));
}

void CIndexBufferOGLBase::Shutdown()
{
    if (IsValid())
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

bool CIndexBufferOGLBase::IsValid() const
{
    return (m_Id != 0);
}

uint64_t CIndexBufferOGLBase::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGLBase::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize(newSize);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    Unbind();
    //assert(glGetError() == GL_NO_ERROR);
}

uint64_t CIndexBufferOGLBase::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGLBase::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

IIndexBuffer::SIndexStream& CIndexBufferOGLBase::Lock()
{
    return m_Stream;
}

bool CIndexBufferOGLBase::IsLocked() const
{
    return m_IsLocked;
}

void CIndexBufferOGLBase::Unlock(bool isNeedCommit)
{
    if (isNeedCommit)
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Buffer.size(), m_Buffer.data());
        Unbind();
        //assert(glGetError() == GL_NO_ERROR);
    }
    m_IsLocked = false;
}

void CIndexBufferOGLBase::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void CIndexBufferOGLBase::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGLBase::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGLBase) || defined(RENDER_OGLES2_0) */
