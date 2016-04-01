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

const uint64_t CBatch::k_MaxBufferSize = 36 * 14;

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
        maxVertexBufferSize == 0)
    {
        return false;
    }
    
    m_BatchedMesh = GRenderer->CreateMesh();
    IVertexBufferPtr vertexBuffer = GRenderer->CreatVertexBuffer();
    
    vertexBuffer->Initialize(elementVertexSize);
    vertexBuffer->Resize(maxVertexBufferSize);
    
    IIndexBufferPtr indexBuffer = nullptr;
    if (elementIndexSize > 0 && maxIndexBufferSize > 0)
    {
        indexBuffer = GRenderer->CreateIndexBuffer();
        
        indexBuffer->Initialize(IIndexBuffer::Short); // TODO:
        indexBuffer->Resize(maxIndexBufferSize);
    }
    
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
    if (!IsInitialized() ||
        !srcVertexBuffer ||
        !srcVertexBuffer->IsValid() ||
        //!CompareCapability(srcVertexBuffer, dstVertexBuffer) ||
        static_cast<bool>(srcIndexBuffer) != static_cast<bool>(dstIndexBuffer)) // TODO: if (srcIB && !dstIB) || (!srcIB && dstIB)
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
    if (!IsInitialized())
    {
        return;
    }
    
    TGeometries::iterator it = m_Geometries.find(mesh->UniqueId());
    if (it != m_Geometries.end())
    {
        const SGeometry& geometry = it->second;
        
        // Remove vertices
        IVertexBufferPtr vertexBuffer = m_BatchedMesh->VertexBuffer();
        uint64_t sizeToCopyVB = geometry.offsetVB + geometry.sizeVB;
        
        uint8_t* rawVB = static_cast<uint8_t*>(vertexBuffer->LockRaw()) + geometry.offsetVB;
        memcpy(rawVB, rawVB + geometry.sizeVB, sizeToCopyVB);
        
        // Remove indices
        IIndexBufferPtr indexBuffer = m_BatchedMesh->IndexBuffer();
        if (indexBuffer)
        {
            short index;
            IIndexBuffer::SIndexStream& dstStreamIB = indexBuffer->Lock();
            uint64_t startIndex = geometry.offsetIB / dstStreamIB.DataSize();
            uint64_t sizeIndex = geometry.sizeIB / dstStreamIB.DataSize();
            for (uint64_t i = 0; i < sizeIndex; ++i)
            {
                dstStreamIB.GetUnsafe<short>(i + startIndex, index);
                index -= sizeIndex;
                dstStreamIB.SetUnsafe<short>(i + startIndex, index);
            }
            
            uint64_t sizeToCopyIB = geometry.offsetIB + geometry.sizeIB;
            
            uint8_t* rawIB = static_cast<uint8_t*>(indexBuffer->LockRaw()) + geometry.offsetIB;
            memcpy(rawIB, rawIB + geometry.sizeIB, sizeToCopyIB);
        }
        
        // Update offsets
        std::for_each(m_Geometries.begin(), m_Geometries.end(), [&](TGeometries::value_type& value)
        {
            SGeometry& geometry = value.second;
            geometry.offsetVB -= geometry.sizeVB;
            geometry.offsetIB -= geometry.sizeIB;
        });
        
        m_VertexOffset -= geometry.sizeVB;
        m_IndexOffset -= geometry.sizeIB;
        
        m_Geometries.erase(it);
    }
}

void CBatch::CopyToBuffer(IMeshPtr mesh, const CTransform3Df& transform)
{
    // Copy vertices and apply transformation to vertex position in batched buffer
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    IVertexBufferPtr srcVertexBuffer = mesh->VertexBuffer();

    const IVertexBuffer::TVertexStreamMap& srcStreamsVB = srcVertexBuffer->VertexStreams();
    std::for_each(srcStreamsVB.begin(), srcStreamsVB.end(), [&](const IVertexBuffer::TVertexStreamMap::value_type& value)
    {
        const IVertexBuffer::SVertexStream& srcStreamVB = value.second;
        IVertexBuffer::SVertexStream& dstStreamVB = dstVertexBuffer->Lock(value.first);
        dstStreamVB.vertexBuffer = dstVertexBuffer;
        dstStreamVB.attributeIndex = srcStreamVB.attributeIndex;
        dstStreamVB.dataType = srcStreamVB.dataType;
        dstStreamVB.stride = srcStreamVB.stride;
        dstStreamVB.offset = srcStreamVB.offset;
        dstStreamVB.absoluteOffset = srcStreamVB.absoluteOffset;
        dstStreamVB.streamIndex = srcStreamVB.streamIndex;
        
        dstStreamVB.CopyFrom(srcStreamVB, 0, srcVertexBuffer->Size(), 0);
    });
    
    ApplyTransform(dstVertexBuffer,
                   m_VertexOffset / dstVertexBuffer->ElementSize(),
                   srcVertexBuffer->Size(),
                   transform());

    // Copy indices and recalculate their value in batched buffer
    IIndexBufferPtr dstIndexBuffer = m_BatchedMesh->IndexBuffer();
    IIndexBufferPtr srcIndexBuffer = mesh->IndexBuffer();
    
    if (srcIndexBuffer)
    {
        short index;
        IIndexBuffer::SIndexStream& dstStreamIB = dstIndexBuffer->Lock();
        IIndexBuffer::SIndexStream& srcStreamIB = srcIndexBuffer->Lock();
        uint64_t offsetIndex = m_IndexOffset / dstStreamIB.DataSize();
        for (uint64_t i = 0; i < srcIndexBuffer->Size(); ++i)
        {
            srcStreamIB.GetUnsafe<short>(i, index);
            index += offsetIndex;
            dstStreamIB.SetUnsafe<short>(offsetIndex + i, index);
        }
    }

    SGeometry geometry = SGeometry();
    geometry.transform = transform;
    geometry.delta = CTransform3Df();
    geometry.offsetVB = m_VertexOffset;
    geometry.sizeVB = srcVertexBuffer->SizeRaw();
    geometry.offsetIB = (srcIndexBuffer ? m_IndexOffset : 0);
    geometry.sizeIB = (srcIndexBuffer ? srcIndexBuffer->SizeRaw() : 0);
    m_Geometries[mesh->UniqueId()] = geometry;

    m_VertexOffset += srcVertexBuffer->SizeRaw();
    m_IndexOffset += (srcIndexBuffer ? srcIndexBuffer->SizeRaw() : 0);
    
    dstVertexBuffer->Unlock(true);
    if (dstIndexBuffer)
    {
        dstIndexBuffer->Unlock(true);
    }
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
        streamVB.GetUnsafe<glm::vec3>(offset + i, pos3);
        pos3 = glm::vec3(glm::vec4(pos3, 0) * transform);
        streamVB.SetUnsafe<glm::vec3>(offset + i, pos3);
    }
}

void CBatch::Update()
{
    if (!m_IsDirty ||
        !IsInitialized())
    {
        return;
    }
    
    IVertexBufferPtr dstVertexBuffer = m_BatchedMesh->VertexBuffer();
    std::for_each(m_Geometries.begin(), m_Geometries.end(), [&](TGeometries::value_type& value)
    {
        SGeometry& geometry = value.second;
        if (!geometry.delta.IsZero())
        {
            ApplyTransform(dstVertexBuffer,
                           geometry.offsetVB / dstVertexBuffer->ElementSize(),
                           geometry.sizeVB / dstVertexBuffer->ElementSize(),
                           geometry.delta());
            geometry.transform += geometry.delta;
            geometry.delta = CTransform3Df();
        }
    });
}

