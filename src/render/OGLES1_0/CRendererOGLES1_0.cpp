//
//  CRendererOGLES1_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1_0)

#include "CRendererOGLES1_0.h"

#include "CVertexBufferOGLES1_0.h"
#include "CIndexBufferOGLES1_0.h"
#include "CMaterialOGLES1_0.h"
#include "CTextureOGLES1_0.h"
#include "CMeshOGLES1_0.h"
#include "CShaderOGLES1_0.h"
#include "CShaderProgramOGLES1_0.h"
#include "CFrameBufferOGLES1_0.h"
#include "CRenderTargetColorOGLES1_0.h"
#include "CRenderTargetDepthOGLES1_0.h"
#include "CRenderTargetStencilOGLES1_0.h"
#include "CRenderTargetTextureOGLES1_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererOGLES1_0::CRendererOGLES1_0(IRenderViewPtr renderView)
: CRendererOGLBase(renderView)
{
}

CRendererOGLES1_0::~CRendererOGLES1_0()
{
}

IFrameBufferPtr CRendererOGLES1_0::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGLES1_0(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGLES1_0::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGLES1_0());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGLES1_0::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGLES1_0());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGLES1_0::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGLES1_0());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGLES1_0::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGLES1_0());
    return textureTarget;
}

IVertexBufferPtr CRendererOGLES1_0::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES1_0());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES1_0::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES1_0());
    return indexBuffer;
}

ITexturePtr CRendererOGLES1_0::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES1_0());
    return texture;
}

IMaterialPtr CRendererOGLES1_0::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES1_0());
    return material;
}

IMeshPtr CRendererOGLES1_0::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES1_0());
    return mesh;
}

IShaderPtr CRendererOGLES1_0::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES1_0());
    return shader;
}

IShaderProgramPtr CRendererOGLES1_0::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES1_0());
    return shaderProgram;
}

void CRendererOGLES1_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGLES1_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES1_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    const GLvoid *data = nullptr;
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, data);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGLES1_0) */
