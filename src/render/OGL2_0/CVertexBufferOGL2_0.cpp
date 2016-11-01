//
//  CVertexBufferOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CVertexBufferOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferOGL2_0::CVertexBufferOGL2_0()
{
}

CVertexBufferOGL2_0::~CVertexBufferOGL2_0()
{
}

void CVertexBufferOGL2_0::Bind()
{
    CVertexBufferOGLBase::Bind();

    const IVertexBuffer::TVertexStreamMap& vertexStreams = VertexStreams();
    std::for_each(vertexStreams.begin(), vertexStreams.end(),
        [&](const IVertexBuffer::TVertexStreamMap::value_type& value) {
            const SVertexStream& stream = value.second;
            if (stream.IsActive()) {
                GLbyte* offset = nullptr;
                offset += (ZeroStride() ? stream.absoluteOffset : stream.offset);
                int type = ConvertDataType(stream.dataType);
                GLsizei elementSize = (ZeroStride() ? 0 : (GLsizei)ElementSize());

                if (stream.attributeIndex != -1ul) {
                    glEnableVertexAttribArray(stream.attributeIndex);
                    glVertexAttribPointer(stream.attributeIndex,
                        stream.stride,
                        type,
                        GL_FALSE,
                        elementSize,
                        (GLvoid*)offset);
                }
            }
        });
}

void CVertexBufferOGL2_0::Unbind()
{
    CVertexBufferOGLBase::Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL2_0) */


