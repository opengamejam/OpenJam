//
//  CMeshOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CMeshOGL1_3.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGL1_3::CMeshOGL1_3()
: m_VertexBuffer(nullptr)
, m_IndexBuffer(nullptr)
{
    
}

CMeshOGL1_3::~CMeshOGL1_3()
{
    
}

void CMeshOGL1_3::Bind()
{
    if (VertexBuffer())
    {
        VertexBuffer()->Bind();
    }
    if (IndexBuffer())
    {
        IndexBuffer()->Bind();
    }
}

void CMeshOGL1_3::Unbind()
{
    if (VertexBuffer())
    {
        VertexBuffer()->Unbind();
    }
    if (IndexBuffer())
    {
        IndexBuffer()->Unbind();
    }
}

bool CMeshOGL1_3::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshOGL1_3::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGL1_3::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGL1_3::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGL1_3::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3