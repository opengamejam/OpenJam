//
//  CVertexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CVertexBufferOGL1_3.h"
#include "CVector3d.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL1_3::CVertexBufferOGL1_3()
: m_Id(0)
, m_ElementSize(0)
, m_IsLocked(false)
, m_ZeroStride(false)
{
    
}

CVertexBufferOGL1_3::~CVertexBufferOGL1_3()
{
    Shutdown();
}

void CVertexBufferOGL1_3::Initialize(size_t elementSize)
{
    if (!IsValid())
    {
#ifdef GL_ARRAY_BUFFER
        glGenBuffers(1, &m_Id);
#else
        m_Id = 1;
#endif
    }
    ElementSize(elementSize);
}

const IVertexBuffer::TVertexStreamMap& CVertexBufferOGL1_3::VertexStreams() const
{
    return m_VertexStreamers;
}

IVertexBuffer::SVertexStream& CVertexBufferOGL1_3::Lock(IVertexBuffer::VertexTypes vertexType)
{
    LockRaw();
    
    if (m_VertexStreamers.find(vertexType) == m_VertexStreamers.end())
    {
        size_t absoluteOffset = 0;
        std::for_each(m_VertexStreamers.begin(), m_VertexStreamers.end(), [&](const TVertexStreamMap::value_type& value)
        {
            const IVertexBuffer::SVertexStream& stream = value.second;
            absoluteOffset += (stream.DataSize() * stream.stride * Size());
        });
        
        IVertexBuffer::SVertexStream stream = IVertexBuffer::SVertexStream(shared_from_this());
        stream.streamIndex = m_VertexStreamers.size();
        stream.absoluteOffset = absoluteOffset;
        
        m_VertexStreamers[vertexType] = stream;
    }
    
    return m_VertexStreamers[vertexType];
}

void CVertexBufferOGL1_3::Shutdown()
{
    if (IsValid())
    {
#ifdef GL_ARRAY_BUFFER
        glDeleteBuffers(1, &m_Id);
#endif
        m_Id = 0;
    }
}

bool CVertexBufferOGL1_3::IsValid() const
{
    return (m_Id != 0);
}

size_t CVertexBufferOGL1_3::SizeRaw() const
{
    return m_Buffer.size();
}

void CVertexBufferOGL1_3::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CVertexBufferOGL1_3::ElementSize() const
{
    return m_ElementSize;
}

void* CVertexBufferOGL1_3::LockRaw()
{
    m_IsLocked = true;
    return m_Buffer.data();
}

bool CVertexBufferOGL1_3::IsLocked() const
{
    return m_IsLocked;
}

void CVertexBufferOGL1_3::Unlock(bool isNeedCommit)
{
    if (!m_IsLocked)
    {
        return;
    }
    
#ifdef GL_ARRAY_BUFFER
    if (isNeedCommit)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Id);
        glBufferData(GL_ARRAY_BUFFER, m_Buffer.size(), m_Buffer.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
#endif
    
    m_IsLocked = false;
}

bool CVertexBufferOGL1_3::HasStream(IVertexBuffer::VertexTypes vertexType)
{
    return (m_VertexStreamers.find(vertexType) != m_VertexStreamers.end());
}

void CVertexBufferOGL1_3::ZeroStride(bool isZeroStride)
{
    m_ZeroStride = isZeroStride;
}

bool CVertexBufferOGL1_3::ZeroStride()
{
    return m_ZeroStride;
}

void CVertexBufferOGL1_3::Bind()
{
#ifdef GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
#endif
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        const SVertexStream& stream = value.second;
        if (stream.IsActive())
        {
            GLubyte *offset = nullptr;
#if defined(OS_KOS)
            offset = (GLubyte *)m_Buffer.data();
#endif
            offset += (ZeroStride() ? stream.absoluteOffset : stream.offset);
            int type = ConvertDataType(stream.dataType);
            GLsizei elementSize = (ZeroStride() ? 0 : (GLsizei)ElementSize());

            if (value.first == IVertexBuffer::Position)
            {
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(stream.stride, type, elementSize, (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::TextureCoords)
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(stream.stride, type, elementSize, (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Color)
            {
            	// TODO: glColorPointer doesn't work properly
                //glEnableClientState(GL_COLOR_ARRAY);
                //glColorPointer(stream.stride, type, elementSize, (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Normal)
            {
                glEnableClientState(GL_NORMAL_ARRAY);
                glNormalPointer(type, elementSize, (GLvoid*)offset);
            }
            else if (value.first == IVertexBuffer::Tangent)
            {
            }
        }
    });

    Unlock();
}

void CVertexBufferOGL1_3::Unbind()
{
#ifdef GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        if (value.second.IsActive())
        {
            if (value.first == IVertexBuffer::Position)
            {
                glDisableClientState(GL_VERTEX_ARRAY);
            }
            else if (value.first == IVertexBuffer::TextureCoords)
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else if (value.first == IVertexBuffer::Color)
            {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            else if (value.first == IVertexBuffer::Normal)
            {
                glDisableClientState(GL_NORMAL_ARRAY);
            }
            else if (value.first == IVertexBuffer::Tangent)
            {
            }
        }
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CVertexBufferOGL1_3::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

int CVertexBufferOGL1_3::ConvertDataType(DataTypes dataType)
{
    static std::map<DataTypes, int> converter = {
        {Unknown, GL_BYTE},
        {Byte, GL_BYTE},
        {UByte, GL_UNSIGNED_BYTE},
        {Short, GL_SHORT},
        {UShort, GL_UNSIGNED_SHORT},
        {Int, GL_INT},
        {UInt, GL_UNSIGNED_INT},
        {Float, GL_FLOAT},
        {ShortFloat, GL_2_BYTES}, // TODO
    };
    
    return converter[dataType];
}

#endif // RENDER_OGL1_3
