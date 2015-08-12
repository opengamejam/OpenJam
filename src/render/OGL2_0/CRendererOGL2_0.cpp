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
#include "CFrameBufferTargetOGL2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL int CovertPrimitiveType(IMaterial::PrimitiveTypes type);

CRendererOGL2_0::CRendererOGL2_0(IRenderViewPtr renderView)
: m_RenderView(renderView)
{

}

CRendererOGL2_0::~CRendererOGL2_0()
{

}

IRenderViewPtr CRendererOGL2_0::RenderView() const
{
    return m_RenderView;
}

IVertexBufferPtr CRendererOGL2_0::CreatVertexBuffer()
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

IRenderTargetPtr CRendererOGL2_0::CreateRenderTarget(unsigned int width, unsigned int height)
{
    IRenderTargetPtr renderTarget(new CFrameBufferTargetOGL2_0(width, height));
    return renderTarget;
}

void CRendererOGL2_0::Draw(IMeshPtr mesh, IMaterialPtr material, IShaderProgramPtr shader)
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

void CRendererOGL2_0::Draw(IVertexBufferPtr vertexBuffer, IMaterialPtr material)
{
    if (!vertexBuffer ||
        !material)
    {
        return;
    }
    
    int primitiveType = CovertPrimitiveType(material->PrimitiveType());
    glDrawArrays(primitiveType, 0, (GLsizei)vertexBuffer->Size());
}

void CRendererOGL2_0::Draw(IVertexBufferPtr vertexBuffer, IIndexBufferPtr indexBuffer, IMaterialPtr material)
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

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0