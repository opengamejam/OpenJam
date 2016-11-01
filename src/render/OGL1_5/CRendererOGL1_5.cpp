//
//  CRendererOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5)

#include "CRendererOGL1_5.h"

#include "CVertexBufferOGL1_5.h"
#include "CIndexBufferOGL1_5.h"
#include "CMaterialOGL1_5.h"
#include "CTextureOGL1_5.h"
#include "CMeshOGL1_5.h"
#include "CShaderOGL1_5.h"
#include "CShaderProgramOGL1_5.h"
#include "CFrameBufferOGL1_5.h"
#include "CRenderTargetColorOGL1_5.h"
#include "CRenderTargetDepthOGL1_5.h"
#include "CRenderTargetStencilOGL1_5.h"
#include "CRenderTargetTextureOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererOGL1_5::CRendererOGL1_5(IRenderViewPtr renderView)
    : CRendererOGLBase(renderView)
{
}

CRendererOGL1_5::~CRendererOGL1_5()
{
}

IFrameBufferPtr CRendererOGL1_5::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGL1_5(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGL1_5::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGL1_5());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGL1_5::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGL1_5());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGL1_5::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGL1_5());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGL1_5::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGL1_5());
    return textureTarget;
}

IVertexBufferPtr CRendererOGL1_5::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGL1_5());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGL1_5::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGL1_5());
    return indexBuffer;
}

ITexturePtr CRendererOGL1_5::CreateTexture()
{
    ITexturePtr texture(new CTextureOGL1_5());
    return texture;
}

IMaterialPtr CRendererOGL1_5::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGL1_5());
    return material;
}

IMeshPtr CRendererOGL1_5::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGL1_5());
    return mesh;
}

IShaderPtr CRendererOGL1_5::CreateShader()
{
    IShaderPtr shader(new CShaderOGL1_5());
    return shader;
}

IShaderProgramPtr CRendererOGL1_5::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGL1_5());
    return shaderProgram;
}

void CRendererOGL1_5::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh || !material) {
        return;
    }

    const IShaderProgram::TUniMatrix4Float& uniforms = shader->UniformsMatrix4x4f();
    IShaderProgram::TUniMatrix4Float::const_iterator it = uniforms.find(shader->ProjectionMatrix());
    glMatrixMode(GL_PROJECTION);
    if (it != uniforms.end()) {
        glLoadMatrixf(glm::value_ptr(it->second));
    } else {
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
    }

    glMatrixMode(GL_MODELVIEW);
    it = uniforms.find(shader->ModelMatrix());
    if (it != uniforms.end()) {
        glLoadMatrixf(glm::value_ptr(it->second));
    } else {
        glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
    }

    if (mesh->IndexBuffer()) {
        Draw(mesh->VertexBuffer(), mesh->IndexBuffer(), material);
    } else {
        Draw(mesh->VertexBuffer(), material);
    }
}

void CRendererOGL1_5::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer || !material) {
        return;
    }

    int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL1_5::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer || !indexBuffer || !material) {
        return;
    }

    int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_SHORT, nullptr);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* defined(RENDER_OGL1_5) */
