//
//  CRendererOGLES2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2_0)

#include "CRendererOGLES2_0.h"

#include "CVertexBufferOGLES2_0.h"
#include "CIndexBufferOGLES2_0.h"
#include "CMaterialOGLES2_0.h"
#include "CTextureOGLES2_0.h"
#include "CMeshOGLES2_0.h"
#include "CShaderOGLES2_0.h"
#include "CShaderProgramOGLES2_0.h"
#include "CFrameBufferTargetOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGLES2_0::CRendererOGLES2_0(IRenderViewPtr renderView)
: m_RenderView(renderView)
{
    
}

CRendererOGLES2_0::~CRendererOGLES2_0()
{
    
}

IRenderViewPtr CRendererOGLES2_0::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGLES2_0::CreatVertexBuffer()
{
    IVertexBufferPtr vertexBuffer(new CVertexBufferOGLES2_0());
    return vertexBuffer;
}

IIndexBufferPtr CRendererOGLES2_0::CreateIndexBuffer()
{
    IIndexBufferPtr indexBuffer(new CIndexBufferOGLES2_0());
    return indexBuffer;
}

ITexturePtr CRendererOGLES2_0::CreateTexture()
{
    ITexturePtr texture(new CTextureOGLES2_0());
    return texture;
}

IMaterialPtr CRendererOGLES2_0::CreateMaterial()
{
    IMaterialPtr material(new CMaterialOGLES2_0());
    return material;
}

IMeshPtr CRendererOGLES2_0::CreateMesh()
{
    IMeshPtr mesh(new CMeshOGLES2_0());
    return mesh;
}

IShaderPtr CRendererOGLES2_0::CreateShader()
{
    IShaderPtr shader(new CShaderOGLES2_0());
    return shader;
}

IShaderProgramPtr CRendererOGLES2_0::CreateShaderProgram()
{
    IShaderProgramPtr shaderProgram(new CShaderProgramOGLES2_0());
    return shaderProgram;
}

IRenderTargetPtr CRendererOGLES2_0::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGLES2_0(width, height));
    return renderTarget;
}

void CRendererOGLES2_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
{
    if (!mesh ||
        !material)
    {
        return;
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

void CRendererOGLES2_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGLES2_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !indexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawElements(primitiveType, (GLsizei)indexBuffer->Size(), GL_UNSIGNED_INT, (void *)0);
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
        case IMaterial::PT_Points:
            primitiveType = GL_POINTS;
            break;
            
        case IMaterial::PT_Lines:
            primitiveType = GL_LINES;
            break;
            
        case IMaterial::PT_LinesLoop:
            primitiveType = GL_LINE_LOOP;
            break;
            
        case IMaterial::PT_LinesStrip:
            primitiveType = GL_LINE_STRIP;
            break;
            
        case IMaterial::PT_Triangles:
            primitiveType = GL_TRIANGLES;
            break;
            
        case IMaterial::PT_TrianglesFan:
            primitiveType = GL_TRIANGLE_FAN;
            break;
            
        case IMaterial::PT_TrianglesStrip:
            primitiveType = GL_TRIANGLE_STRIP;
            break;
            
        default:
            break;
    }
    
    return primitiveType;
}


#endif // RENDER_OGLES2_0