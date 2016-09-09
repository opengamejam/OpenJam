
//
//  CRendererOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CRendererOGL1_3.h"

#include "CVertexBufferOGL1_3.h"
#include "CIndexBufferOGL1_3.h"
#include "CMaterialOGL1_3.h"
#include "CTextureOGL1_3.h"
#include "CMeshOGL1_3.h"
#include "CShaderOGL1_3.h"
#include "CShaderProgramOGL1_3.h"
#include "CFrameBufferOGL1_3.h"
#include "CRenderTargetColorOGL1_3.h"
#include "CRenderTargetDepthOGL1_3.h"
#include "CRenderTargetStencilOGL1_3.h"
#include "CRenderTargetTextureOGL1_3.h"
#include "glm/ext.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGL1_3::CRendererOGL1_3(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGL1_3::~CRendererOGL1_3()
{
    
}

IRenderViewPtr CRendererOGL1_3::RenderView() const
{
    return m_RenderView;
}

IFrameBufferPtr CRendererOGL1_3::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGL1_3(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGL1_3::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGL1_3());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGL1_3::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGL1_3());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGL1_3::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGL1_3());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGL1_3::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGL1_3());
    return textureTarget;
}

IVertexBufferPtr CRendererOGL1_3::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGL1_3());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGL1_3::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGL1_3());
    return indexBuffer;
}

ITexturePtr CRendererOGL1_3::CreateTexture()
{
    ITexturePtr texture(new CTextureOGL1_3());
    return texture;
}

IMaterialPtr CRendererOGL1_3::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGL1_3());
    return material;
}

IMeshPtr CRendererOGL1_3::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGL1_3());
    return mesh;
}

IShaderPtr CRendererOGL1_3::CreateShader()
{
    IShaderPtr shader(new CShaderOGL1_3());
    return shader;
}

IShaderProgramPtr CRendererOGL1_3::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGL1_3());
    return shaderProgram;
}

void CRendererOGL1_3::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh ||
        !material)
    {
        return;
    }
    
    const IShaderProgram::TUniMatrix4Float& uniforms = shader->UniformsMatrix4x4f();
    IShaderProgram::TUniMatrix4Float::const_iterator it = uniforms.find(shader->ProjectionMatrix());
    
    glMatrixMode(GL_PROJECTION);
	if (it != uniforms.end())
	{
		glLoadMatrixf(glm::value_ptr(it->second));
	}
	else
	{
		glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
	}

	glMatrixMode(GL_MODELVIEW);
	it = uniforms.find(shader->ModelMatrix());
	if (it != uniforms.end())
	{
		glLoadMatrixf(glm::value_ptr(it->second));
	}
	else
	{
		glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
	}
    
    if (mesh->IndexBuffer())
    {
        Draw(mesh->VertexBuffer(), mesh->IndexBuffer(), material);
    }
    else
    {
        Draw(mesh->VertexBuffer(), material);
    }
}

void CRendererOGL1_3::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL1_3::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    const GLvoid *data = nullptr;
#if	defined(OS_KOS)
    data = indexBuffer->LockRaw();
#endif
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, data);
#if defined(OS_KOS)
    indexBuffer->Unlock();
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type)
{
    int primitiveType = GL_TRIANGLES;
    switch (type)
    {
        case IMaterial::Points:
            primitiveType = GL_POINTS;
            break;

        case IMaterial::Lines:
            primitiveType = GL_LINES;
            break;

        case IMaterial::LinesLoop:
            primitiveType = GL_LINE_LOOP;
            break;

        case IMaterial::LinesStrip:
            primitiveType = GL_LINE_STRIP;
            break;

        case IMaterial::Triangles:
            primitiveType = GL_TRIANGLES;
            break;

        case IMaterial::TrianglesFan:
            primitiveType = GL_TRIANGLE_FAN;
            break;

        case IMaterial::TrianglesStrip:
            primitiveType = GL_TRIANGLE_STRIP;
            break;

        default:
            break;
    }

    return primitiveType;
}

#endif // RENDER_OGL1_3
