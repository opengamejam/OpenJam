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

const uint64_t CBatch::k_MaxVertexBufferSize = 10000;

CBatch::CBatch()
: m_IsDirty(true)
, m_BatchedMesh(nullptr)
, m_Material(nullptr)
, m_ShaderProgram(nullptr)
, m_VertexOffset(0)
, m_IndexOffset(0)
{

}

CBatch::~CBatch()
{

}

bool CBatch::Initialize(IMaterialPtr material,
                        IShaderProgramPtr shader,
                        const std::list<ITexturePtr>& textures,
                        uint64_t elementSize,
                        uint64_t maxVertexBufferSize)
{
    if (IsInitialized() ||
        !material ||
        !shader ||
        elementSize == 0 ||
        maxVertexBufferSize == 0)
    {
        return false;
    }
    
    m_BatchedMesh = GRenderer->CreateMesh();
    IVertexBufferPtr vertexBuffer = GRenderer->CreatVertexBuffer();
    IIndexBufferPtr indexBuffer = GRenderer->CreateIndexBuffer();
    
    vertexBuffer->Initialize(elementSize);
    vertexBuffer->Resize(maxVertexBufferSize);
    
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
    
    if (m_BatchedMesh->VertexBuffer())
    {
        m_BatchedMesh->VertexBuffer()->Shutdown();
    }
    if (m_BatchedMesh->IndexBuffer())
    {
        m_BatchedMesh->IndexBuffer()->Shutdown();
    }
    m_BatchedMesh = nullptr;
    m_Material = nullptr;
    m_ShaderProgram = nullptr;
    m_Textures.clear();
    m_Geometries.clear();
    m_VertexOffset = 0;
    m_IndexOffset = 0;
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

const std::list<ITexturePtr>& CBatch::Textures() const
{
    return m_Textures;
}

bool CBatch::AddGeometry(IMeshPtr mesh, const CTransform3Df& transform)
{
    IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();
    IIndexBufferPtr srcIndexBuffer = mesh->IndexBuffer();
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    IIndexBufferPtr dstIndexBuffer = m_BatchedMesh->IndexBuffer();
    if (!IsInitialized() || !srcVertexBuffer || !srcVertexBuffer->IsValid())
    {
        return false;
    }
    
    TGeometries::iterator it = m_Geometries.find(mesh->UniqueId());
    if (it == m_Geometries.end())
    {
        m_Geometries[mesh->UniqueId()] = SGeometry(m_VertexOffset,
                                                   mesh->VertexBuffer()->SizeRaw(),
                                                   m_IndexOffset,
                                                   mesh->IndexBuffer()->SizeRaw(),
                                                   transform,
                                                   mesh);
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
    
    m_VertexOffset = 0;
    m_IndexOffset = 0;
    
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    IIndexBufferPtr dstIndexBuffer = m_BatchedMesh->IndexBuffer();
    
    char* dstRawVB = static_cast<char *>(dstVertexBuffer->LockRaw());
    char* dstRawIB = static_cast<char *>(dstIndexBuffer->LockRaw());
    
    IVertexBuffer::SVertexStream& dstStreamVB = dstVertexBuffer->Lock(IVertexBuffer::Position);
    std::for_each(m_Geometries.begin(), m_Geometries.end(), [&](TGeometries::value_type& value)
    {
        SGeometry& geometry = value.second;
        IMeshPtr mesh = geometry.mesh.lock();
        if (!mesh)
        {
            geometry.offsetVB = 0;
            geometry.sizeVB = 0;
            geometry.offsetIB = 0;
            geometry.sizeIB = 0;
            
            return;
        }
        
        // Copy raw data to batched vertex/index buffers
        IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();
        IIndexBufferPtr srcIndexBuffer = mesh->IndexBuffer();
        
        char* srcRawVB = static_cast<char *>(srcVertexBuffer->LockRaw());
        char* srcRawIB = static_cast<char *>(srcIndexBuffer->LockRaw());
        
        memcpy(srcRawVB, dstRawVB + m_VertexOffset, srcVertexBuffer->SizeRaw());
        memcpy(srcRawIB, dstRawIB + m_IndexOffset, srcIndexBuffer->SizeRaw());
        
        // Apply transformation to vertex position
        glm::vec3 pos3;
        glm::mat4 transform = geometry.transform();
        for (uint64_t i = 0; i < srcVertexBuffer->Size(); ++i)
        {
            dstStreamVB.GetUnsafe<glm::vec3>(dstVertexBuffer, m_VertexOffset + i, pos3);
            pos3 = glm::vec3(glm::vec4(pos3, 0) * transform);
            dstStreamVB.SetUnsafe<glm::vec3>(dstVertexBuffer, m_VertexOffset + i, pos3);
        }
        
        // Recalculate indexes in batched buffer
        for (uint64_t i = 0; i < srcIndexBuffer->Size(); ++i)
        {
            ((short *)srcRawIB)[i] += m_IndexOffset;
        }
        
        geometry.offsetVB = m_VertexOffset;
        geometry.sizeVB = srcVertexBuffer->SizeRaw();
        geometry.offsetIB = m_IndexOffset;
        geometry.sizeIB = srcIndexBuffer->SizeRaw();
        
        m_VertexOffset += srcVertexBuffer->SizeRaw();
        m_IndexOffset += srcIndexBuffer->SizeRaw();
    });
    
    dstVertexBuffer->Unlock();
    dstIndexBuffer->Unlock();
}

