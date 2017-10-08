//
//  CVertexBufferOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CVertexBufferOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGLBase::CVertexBufferOGLBase()
    : m_Id(0)
    , m_ElementSize(0)
    , m_IsLocked(false)
    , m_ZeroStride(false)
{
}

CVertexBufferOGLBase::~CVertexBufferOGLBase()
{
    Shutdown();
}

void CVertexBufferOGLBase::Initialize(uint64_t elementSize)
{
    if (!IsValid()) {
#ifdef GL_ARRAY_BUFFER
        glGenBuffers(1, &m_Id);
#else
        m_Id = 1;
#endif
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGLBase::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGLBase::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();

    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end()) {
        uint64_t absoluteOffset = 0;
        std::for_each(m_VertexStreamers.begin(), m_VertexStreamers.end(), [&](const TVertexStreamMap::value_type& value) {
            const IVertexBuffer::SVertexStream& stream = value.second;
            absoluteOffset += (stream.DataSize() * Size());
        });

        SVertexStream stream = SVertexStream(shared_from_this());
        stream.streamIndex = (unsigned int)m_VertexStreamers.size();
        stream.absoluteOffset = absoluteOffset;

        m_VertexStreamers[vertexType] = stream;
    }

    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGLBase::Shutdown()
{
    if (IsValid()) {
#ifdef GL_ARRAY_BUFFER
        glDeleteBuffers(1, &m_Id);
#endif
        m_Id = 0;
    }
}

bool CVertexBufferOGLBase::IsValid() const
{
    return (m_Id != 0);
}

uint64_t CVertexBufferOGLBase::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGLBase::ResizeRaw(uint64_t newSize)
{
    m_Buffer.resize((size_t)newSize);
#ifdef GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    //assert(glGetError() == GL_NO_ERROR);
}

uint64_t CVertexBufferOGLBase::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGLBase::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGLBase::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGLBase::Unlock(bool isNeedCommit)
{
    if (!m_IsLocked) {
        return;
    }

#ifdef GL_ARRAY_BUFFER
    if (isNeedCommit) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_Buffer.size(), m_Buffer.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //assert(glGetError() == GL_NO_ERROR);
    }
#endif

    m_IsLocked = false;
}

bool CVertexBufferOGLBase::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGLBase::ZeroStride(bool isZeroStride)
{
    m_ZeroStride = isZeroStride;
}

bool CVertexBufferOGLBase::ZeroStride()
{
    return m_ZeroStride;
}

void CVertexBufferOGLBase::Bind()
{
#ifdef GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
#endif
}

void CVertexBufferOGLBase::Unbind()
{
#ifdef GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

GLenum CVertexBufferOGLBase::ConvertDataType(DataTypes dataType)
{
    static std::map<DataTypes, int> converter = {
        { Unknown, GL_BYTE },
        { Byte, GL_BYTE },
        { UByte, GL_UNSIGNED_BYTE },
        { Short, GL_SHORT },
        { UShort, GL_UNSIGNED_SHORT },
        { Int, GL_INT },
        { UInt, GL_UNSIGNED_INT },
        { Float, GL_FLOAT },
        //{ShortFloat, GL_2_BYTES}, // TODO
    };

    return converter[dataType];
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGLBase::ElementSize(uint64_t elementSize)
{
    m_ElementSize = std::max<uint64_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
