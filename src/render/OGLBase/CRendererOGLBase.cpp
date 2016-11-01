//
//  CRendererOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CRendererOGLBase.h"
#include "CMeshOGLBase.h"
#include "CVertexBufferOGLBase.h"
#include "CIndexBufferOGLBase.h"
#include "CMaterialOGLBase.h"
#include "CTextureOGLBase.h"
#include "CMeshOGLBase.h"
#include "CShaderOGLBase.h"
#include "CShaderProgramOGLBase.h"
#include "CFrameBufferOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererOGLBase::CRendererOGLBase(IRenderViewPtr renderView)
    : m_RenderView(renderView)
{
}

CRendererOGLBase::~CRendererOGLBase()
{
}

IRenderViewPtr CRendererOGLBase::RenderView() const
{
    return m_RenderView;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

GLenum CRendererOGLBase::ConvertPrimitiveType(IMaterial::PrimitiveTypes type)
{
    switch (type) {
    case IMaterial::Points:
        return GL_POINTS;
        break;

    case IMaterial::Lines:
        return GL_LINES;
        break;

    case IMaterial::LinesLoop:
        return GL_LINE_LOOP;
        break;

    case IMaterial::LinesStrip:
        return GL_LINE_STRIP;
        break;

    case IMaterial::Triangles:
        return GL_TRIANGLES;
        break;

    case IMaterial::TrianglesFan:
        return GL_TRIANGLE_FAN;
        break;

    case IMaterial::TrianglesStrip:
        return GL_TRIANGLE_STRIP;
        break;

    default:
        return GL_TRIANGLES;
        break;
    }
}

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
