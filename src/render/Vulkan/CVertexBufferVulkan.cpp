//
//  CVertexBufferVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#include "CVertexBufferVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CVertexBufferVulkan::CVertexBufferVulkan()
{
}

CVertexBufferVulkan::~CVertexBufferVulkan()
{
}

void CVertexBufferVulkan::Bind()
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

void CVertexBufferVulkan::Unbind()
{
    CVertexBufferOGLBase::Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_VULKAN) */
