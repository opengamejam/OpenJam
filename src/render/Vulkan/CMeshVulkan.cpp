//
//  CMeshVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_VULKAN)

#include "CMeshVulkan.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMeshVulkan::CMeshVulkan()
    : m_VAO(0)
{
#ifdef GL_GLEXT_PROTOTYPES // TODO:
    glGenVertexArraysOES(1, &m_VAO);
#endif
}

CMeshVulkan::~CMeshVulkan()
{
#ifdef GL_GLEXT_PROTOTYPES
    glDeleteVertexArraysOES(1, &m_VAO);
#endif
}

void CMeshVulkan::Bind()
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

void CMeshVulkan::Unbind()
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

bool CMeshVulkan::IsValid() const
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

#endif // RENDER_VULKAN
