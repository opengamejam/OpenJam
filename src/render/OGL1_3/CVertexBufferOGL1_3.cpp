//
//  CVertexBufferOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CVertexBufferOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL1_3::CVertexBufferOGL1_3()
{
}

CVertexBufferOGL1_3::~CVertexBufferOGL1_3()
{
}

void CVertexBufferOGL1_3::Bind()
{
    CVertexBufferOGLBase::Bind();

    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
        [&](const IVertexBuffer::TVertexStreamMap::value_type& value) {
            const SVertexStream& stream = value.second;
            if (stream.IsActive()) {
                GLubyte* offset = nullptr;
#if defined(OS_KOS)
                offset = (GLubyte*)m_Buffer.data();
#endif
                offset += (ZeroStride() ? stream.absoluteOffset : stream.offset);
                int type = ConvertDataType(stream.dataType);
                GLsizei elementSize = (ZeroStride() ? 0 : (GLsizei)ElementSize());

                if (value.first == IVertexBuffer::Position) {
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(stream.stride, type, elementSize, (GLvoid*)offset);
                } else if (value.first == IVertexBuffer::TextureCoords) {
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glTexCoordPointer(stream.stride, type, elementSize, (GLvoid*)offset);
                } else if (value.first == IVertexBuffer::Color) {
                    // TODO: glColorPointer doesn't work properly
                    //glEnableClientState(GL_COLOR_ARRAY);
                    //glColorPointer(stream.stride, type, elementSize, (GLvoid*)offset);
                } else if (value.first == IVertexBuffer::Normal) {
                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(type, elementSize, (GLvoid*)offset);
                } else if (value.first == IVertexBuffer::Tangent) {
                }
            }
        });
}

void CVertexBufferOGL1_3::Unbind()
{
    CVertexBufferOGLBase::Unbind();

    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
        [&](const IVertexBuffer::TVertexStreamMap::value_type& value) {
            if (value.second.IsActive()) {
                if (value.first == IVertexBuffer::Position) {
                    glDisableClientState(GL_VERTEX_ARRAY);
                } else if (value.first == IVertexBuffer::TextureCoords) {
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                } else if (value.first == IVertexBuffer::Color) {
                    glDisableClientState(GL_COLOR_ARRAY);
                } else if (value.first == IVertexBuffer::Normal) {
                    glDisableClientState(GL_NORMAL_ARRAY);
                } else if (value.first == IVertexBuffer::Tangent) {
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

#endif // RENDER_OGL1_3

