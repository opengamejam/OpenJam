//
//  CRendererOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0)

#include "CRendererOGL2_0.h"

#include "CVertexBufferOGL2_0.h"
#include "CIndexBufferOGL2_0.h"
#include "CMaterialOGL2_0.h"
#include "CTextureOGL2_0.h"
#include "CMeshOGL2_0.h"
#include "CShaderOGL2_0.h"
#include "CShaderProgramOGL2_0.h"
#include "CFrameBufferOGL2_0.h"
#include "CRenderTargetColorOGL2_0.h"
#include "CRenderTargetDepthOGL2_0.h"
#include "CRenderTargetStencilOGL2_0.h"
#include "CRenderTargetTextureOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRendererOGL2_0::CRendererOGL2_0(IRenderViewPtr renderView)
    : CRendererOGLBase(renderView)
{
}

CRendererOGL2_0::~CRendererOGL2_0()
{
}

IFrameBufferPtr CRendererOGL2_0::CreateFrameBuffer(uint32_t width, uint32_t height)
{
    IFrameBufferPtr frameBuffer(new CFrameBufferOGL2_0(width, height));
    return frameBuffer;
}

CRenderTargetColorPtr CRendererOGL2_0::CreateColorRenderTarget()
{
    CRenderTargetColorPtr colorTarget(new CRenderTargetColorOGL2_0());
    return colorTarget;
}

CRenderTargetDepthPtr CRendererOGL2_0::CreateDepthRenderTarget()
{
    CRenderTargetDepthPtr depthTarget(new CRenderTargetDepthOGL2_0());
    return depthTarget;
}

CRenderTargetStencilPtr CRendererOGL2_0::CreateStencilRenderTarget()
{
    CRenderTargetStencilPtr stencilTarget(new CRenderTargetStencilOGL2_0());
    return stencilTarget;
}

CRenderTargetTexturePtr CRendererOGL2_0::CreateTextureRenderTarget()
{
    CRenderTargetTexturePtr textureTarget(new CRenderTargetTextureOGL2_0());
    return textureTarget;
}

IVertexBufferPtr CRendererOGL2_0::CreateVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGL2_0());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGL2_0::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGL2_0());
    return indexBuffer;
}

ITexturePtr CRendererOGL2_0::CreateTexture()
{
    ITexturePtr texture(new CTextureOGL2_0());
    return texture;
}

IMaterialPtr CRendererOGL2_0::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGL2_0());
    return material;
}

IMeshPtr CRendererOGL2_0::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGL2_0());
    return mesh;
}

IShaderPtr CRendererOGL2_0::CreateShader()
{
    IShaderPtr shader(new CShaderOGL2_0());
    return shader;
}

IShaderProgramPtr CRendererOGL2_0::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGL2_0());
    return shaderProgram;
}

void CRendererOGL2_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    assert(mesh && material);
    IVertexBufferPtr vertexBuffer = mesh->VertexBuffer();
    IIndexBufferPtr indexBuffer = mesh->IndexBuffer();

    if (indexBuffer) {
        Draw(vertexBuffer, indexBuffer, material);
    } else {
        Draw(vertexBuffer, material);
    }
}

void CRendererOGL2_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer || !material) {
        return;
    }

    int primitiveType = ConvertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL2_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
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

#endif /* defined(RENDER_OGL2_0) */
