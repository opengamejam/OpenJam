//
//  CMeshOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#include "CMeshOGLES2_0.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshOGLES2_0::CMeshOGLES2_0()
    : m_VAO(0)
{
#ifdef GL_GLEXT_PROTOTYPES // TODO:
    glGenVertexArraysOES(1, &m_VAO);
#endif
}

CMeshOGLES2_0::~CMeshOGLES2_0()
{
#ifdef GL_GLEXT_PROTOTYPES
    glDeleteVertexArraysOES(1, &m_VAO);
#endif
}

void CMeshOGLES2_0::Bind()
{
#ifdef GL_GLEXT_PROTOTYPES
    glBindVertexArrayOES(m_VAO);
#else
    if (VertexBuffer()) {
        VertexBuffer()->Bind();
    }
    if (IndexBuffer()) {
        IndexBuffer()->Bind();
    }
#endif
}

void CMeshOGLES2_0::Unbind()
{
#ifdef GL_GLEXT_PROTOTYPES
    glBindVertexArrayOES(0);
#else
    if (VertexBuffer()) {
        VertexBuffer()->Unbind();
    }
    if (IndexBuffer()) {
        IndexBuffer()->Unbind();
    }
#endif
}

bool CMeshOGLES2_0::IsValid() const
{
#ifdef GL_GLEXT_PROTOTYPES
    return (m_VAO != 0);
#else
    return true;
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGLES2_0
