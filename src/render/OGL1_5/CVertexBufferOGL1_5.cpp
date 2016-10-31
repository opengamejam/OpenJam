//
//  CVertexBufferOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#include "CVertexBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL1_5::CVertexBufferOGL1_5()
{
}

CVertexBufferOGL1_5::~CVertexBufferOGL1_5()
{
}

void CVertexBufferOGL1_5::Bind()
{
    CVertexBufferOGLBase::Bind();
    
    GLsizei elementSize = (ZeroStride() ? 0 : (GLsizei)ElementSize());
    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
                  [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        const SVertexStream& stream = value.second;
        if (stream.IsActive())
        {
            GLbyte *offset = nullptr;
            offset += (ZeroStride() ? stream.absoluteOffset : stream.offset);
            int type = ConvertDataType(stream.dataType);
            
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
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(stream.stride, type, elementSize, (GLvoid*)offset);
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
}

void CVertexBufferOGL1_5::Unbind()
{
    CVertexBufferOGLBase::Unbind();
    
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

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_5) */
