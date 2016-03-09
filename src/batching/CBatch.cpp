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

const uint64_t CBatch::k_MaxBufferSize = 10000;

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
                        uint64_t elementVertexSize,
                        uint64_t elementIndexSize,
                        uint64_t maxVertexBufferSize,
                        uint64_t maxIndexBufferSize)
{
    if (IsInitialized() ||
        !material ||
        !shader ||
        elementVertexSize == 0 ||
        maxVertexBufferSize == 0 ||
        elementIndexSize == 0 || // TODO:
        maxIndexBufferSize == 0)
    {
        return false;
    }
    
    m_BatchedMesh = GRenderer->CreateMesh();
    IVertexBufferPtr vertexBuffer = GRenderer->CreatVertexBuffer();
    IIndexBufferPtr indexBuffer = GRenderer->CreateIndexBuffer();
    
    vertexBuffer->Initialize(elementVertexSize);
    vertexBuffer->Resize(maxVertexBufferSize);
    
    indexBuffer->Initialize(IIndexBuffer::Short); // TODO:
    indexBuffer->Resize(maxIndexBufferSize);
    
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
        CopyToBuffer(mesh, transform);
    }
    else
    {
        SGeometry& geometry = it->second;
        geometry.delta = transform - geometry.transform;
        m_IsDirty = true;
    }
    
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

void CBatch::CopyToBuffer(IMeshPtr mesh, const CTransform3Df& transform)
{
    // Copy vertices and apply transformation to vertex position in batched buffer
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();

    char* srcRawVB = static_cast<char *>(srcVertexBuffer->LockRaw());
    char* dstRawVB = static_cast<char *>(dstVertexBuffer->LockRaw());
    memcpy(srcRawVB, dstRawVB + m_VertexOffset, srcVertexBuffer->SizeRaw());
    
    ApplyTransform(dstVertexBuffer, m_VertexOffset, srcVertexBuffer->SizeRaw(), transform());

    // Copy indices and recalculate their value in batched buffer
    IIndexBufferPtr dstIndexBuffer = m_BatchedMesh->IndexBuffer();
    IIndexBufferPtr srcIndexBuffer = mesh->IndexBuffer();
    
    short index;
    IIndexBuffer::SIndexStream& dstStreamIB = dstIndexBuffer->Lock();
    for (uint64_t i = 0; i < srcIndexBuffer->Size(); ++i)
    {
        dstStreamIB.GetUnsafe<short>(srcIndexBuffer, i, index);
        index += m_IndexOffset;
        dstStreamIB.SetUnsafe<short>(dstIndexBuffer, m_IndexOffset + i, index);
    }

    SGeometry geometry = SGeometry();
    geometry.transform = transform;
    geometry.delta = CTransform3Df();
    geometry.offsetVB = m_VertexOffset;
    geometry.sizeVB = srcVertexBuffer->SizeRaw();
    geometry.offsetIB = m_IndexOffset;
    geometry.sizeIB = srcIndexBuffer->SizeRaw();
    m_Geometries[mesh->UniqueId()] = geometry;

    m_VertexOffset += srcVertexBuffer->SizeRaw();
    m_IndexOffset += srcIndexBuffer->SizeRaw();
    
    dstVertexBuffer->Unlock();
    dstIndexBuffer->Unlock();
}

void CBatch::ApplyTransform(IVertexBufferPtr vertexBuffer,
                            uint64_t offset,
                            uint64_t size,
                            glm::mat4 transform)
{
    glm::vec3 pos3;
    IVertexBuffer::SVertexStream& streamVB = vertexBuffer->Lock(IVertexBuffer::Position);
    for (uint64_t i = 0; i < size; ++i)
    {
        streamVB.GetUnsafe<glm::vec3>(vertexBuffer, offset + i, pos3);
        pos3 = glm::vec3(glm::vec4(pos3, 0) * transform);
        streamVB.SetUnsafe<glm::vec3>(vertexBuffer, offset + i, pos3);
    }
    
}

void CBatch::Update()
{
    if (!m_IsDirty)
    {
        return;
    }
    
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    std::for_each(m_Geometries.begin(), m_Geometries.end(), [&](TGeometries::value_type& value)
    {
        SGeometry& geometry = value.second;
        if (!geometry.delta.IsZero())
        {
            ApplyTransform(dstVertexBuffer, geometry.offsetVB, geometry.sizeVB, geometry.delta());
            geometry.transform += geometry.delta;
            geometry.delta = CTransform3Df();
        }
    });
}

