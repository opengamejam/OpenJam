//
//  CBatch.cpp
//  TestApp
//
//  Created by yev on 8/25/15.
//
//

#include "CBatch.h"
#include "RenderGlobal.h"
#include "IMesh.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "IMaterial.h"
#include "IShaderProgram.h"
#include "ITexture.h"

using namespace jam;

const uint64_t k_MaxVertexBufferSize = 10000;

CBatch::CBatch()
: m_IsDirty(true)
, m_BatchedMesh(nullptr)
, m_Material(nullptr)
, m_ShaderProgram(nullptr)
{

}

CBatch::~CBatch()
{

}

bool CBatch::Initialize(IMaterialPtr material, IShaderProgramPtr shader, const std::list<ITexturePtr> textures)
{
    if (IsInitialized() || !material || !shader)
    {
        return false;
    }
    
    m_BatchedMesh = GRenderer->CreateMesh();
    IVertexBufferPtr vertexBuffer = GRenderer->CreatVertexBuffer();
    IIndexBufferPtr indexBuffer = GRenderer->CreateIndexBuffer();
    indexBuffer->Initialize(sizeof(unsigned short));
    
    m_BatchedMesh->VertexBuffer(vertexBuffer);
    m_BatchedMesh->IndexBuffer(indexBuffer);
    
    m_Material = material;
    m_ShaderProgram = shader;
    m_Textures = textures;
    
    return true;
}

bool CBatch::IsInitialized() const
{
    return (m_BatchedMesh != nullptr);
}

void CBatch::Shutdown()
{
    if (!IsInitialized())
    {
        return;
    }
    
    m_BatchedMesh->VertexBuffer()->Shutdown();
    m_BatchedMesh->IndexBuffer()->Shutdown();
    m_BatchedMesh = nullptr;
    m_Material = nullptr;
    m_ShaderProgram = nullptr;
    m_Textures.clear();
    m_Geometries.clear();
}

const IMeshPtr CBatch::Mesh() const
{
    return m_BatchedMesh;
}

const IMaterialPtr CBatch::Material() const
{
    return m_Material;
}

const IShaderProgramPtr CBatch::Shader() const
{
    return m_ShaderProgram;
}

const std::list<ITexturePtr> CBatch::Textures() const
{
    return m_Textures;
}

bool CBatch::AddGeometry(IMeshPtr mesh, const CTransform3Df& transform)
{
    if (!IsInitialized() || !mesh->VertexBuffer() || !mesh->VertexBuffer()->IsValid())
    {
        return false;
    }
    
    IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    
    if (!dstVertexBuffer->IsValid())
    {
        dstVertexBuffer->Initialize(srcVertexBuffer->ElementSize());
        dstVertexBuffer->Resize(k_MaxVertexBufferSize);
    }
    else if (!ValidateStreams(srcVertexBuffer, dstVertexBuffer))
    {
        // Incompatible buffer
        m_Geometries.erase(mesh->UniqueId());
        return false;
    }
    
    TGeometries::iterator it = m_Geometries.find(mesh->UniqueId());
    if (it == m_Geometries.end())
    {
        m_Geometries[mesh->UniqueId()] = SGeometry({mesh, transform});
    }
    m_IsDirty = true;
    
    return true;
}

void CBatch::RemoveGeometry(IMeshPtr mesh)
{
    TGeometries::iterator it = m_Geometries.find(mesh->UniqueId());
    if (it != m_Geometries.end())
    {
        m_Geometries.erase(it);
        
        m_IsDirty = true;
    }
}

void CBatch::Update()
{
    if (!m_IsDirty)
    {
        return;
    }
    
    
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    std::for_each(m_Geometries.begin(), m_Geometries.end(), [&](const TGeometries::value_type& value)
    {
        IMeshPtr mesh = value.second.mesh.lock();
        if (!mesh)
        {
            return;
        }
        IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();
        
        
    });
    
    
}

bool CBatch::ValidateStreams(IVertexBufferPtr vb1, IVertexBufferPtr vb2) const
{
    bool isVertexBufferValid = true;
    
    const IVertexBuffer::TVertexStreamMap& vertexStreams = vb1->VertexStreams();
    std::all_of(vertexStreams.begin(), vertexStreams.end(),
                [&](const IVertexBuffer::TVertexStreamMap::value_type& stream)
    {
        if (!vb2->HasStream(stream.first))
        {
            isVertexBufferValid = false;
            return false;
        }
        
        return true;
    });
    
    return isVertexBufferValid;
}

