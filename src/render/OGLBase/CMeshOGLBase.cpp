//
//  CMeshOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CMeshOGLBase.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGLBase::CMeshOGLBase()
    : m_VertexBuffer(nullptr)
    , m_IndexBuffer(nullptr)
{
}

CMeshOGLBase::~CMeshOGLBase()
{
}

void CMeshOGLBase::Bind()
{
    if (VertexBuffer()) {
        VertexBuffer()->Bind();
    }
    if (IndexBuffer()) {
        IndexBuffer()->Bind();
    }
}

void CMeshOGLBase::Unbind()
{
    if (VertexBuffer()) {
        VertexBuffer()->Unbind();
    }
    if (IndexBuffer()) {
        IndexBuffer()->Unbind();
    }
}

bool CMeshOGLBase::IsValid() const
{
    return true;
}

IVertexBufferPtr CMeshOGLBase::VertexBuffer() const
{
    return m_VertexBuffer;
}

void CMeshOGLBase::VertexBuffer(IVertexBufferPtr vertexBuffer)
{
    m_VertexBuffer = vertexBuffer;
}

IIndexBufferPtr CMeshOGLBase::IndexBuffer() const
{
    return m_IndexBuffer;
}

void CMeshOGLBase::IndexBuffer(IIndexBufferPtr indexBuffer)
{
    m_IndexBuffer = indexBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
