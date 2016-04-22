//
//  CBatchingSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CBatchingSystem.h"
#include "IEntity.h"
#include "CBatchComponent.h"
#include "CTransformationComponent.h"
#include "CRenderComponent.h"
#include "CMathFunc.hpp"
#include "IMaterial.h"
#include "ITexture.h"
#include "IShader.h"
#include "RenderGlobal.h"
#include "CShaderSourceCommon.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CBatchingSystem::CBatchingSystem()
{
    RegisterComponent(ComponentId<CBatchComponent>());
    RegisterComponent(ComponentId<CTransformationComponent>());
}

CBatchingSystem::~CBatchingSystem()
{
    
}

void CBatchingSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        CTransformationComponentPtr transformComp = entity->Get<CTransformationComponent>();
        CBatchComponentPtr batchComp = entity->Get<CBatchComponent>();
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComp)
        {
            if (!batchComp ||
                !renderComp->Batchable())
            {
                return;
            }
            
            SGeometry* geometry = nullptr;
            if (batchComp->BatchId() == 0)
            {
                // Copy to buffer
                const std::set<std::string>& groups = renderComp->Groups();
                std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
                {
                    if (groupName == CRenderComponent::kBatchingGroupName)
                    {
                        return;
                    }
                    
                    IMeshPtr mesh = renderComp->Mesh(groupName);
                    IMaterialPtr material = renderComp->Material(groupName);
                    ITexturePtr texture = renderComp->Texture(groupName);
                    IShaderProgramPtr shader = renderComp->Shader(groupName);
                    
                    std::tuple<std::string, std::string> container(material->Hash(), texture->Hash()); // TODO: shaderId, cameraId
                    uint64_t batchId = hash_tuple(container);
                    
                    TBatchCache::iterator it = m_Batches.find(batchId);
                    if (it == m_Batches.end())
                    {
                        uint64_t elementVertexSize = mesh->VertexBuffer()->ElementSize();
                        uint64_t elementIndexSize = mesh->IndexBuffer() ? mesh->IndexBuffer()->ElementSize() : 0;
                        
                        m_Batches[batchId] = CreateNewGeometry(material,
                                                               texture,
                                                               shader,
                                                               elementVertexSize,
                                                               elementIndexSize);
                    }
                    
                    geometry = &m_Batches[batchId];
                    
                    uint64_t offsetVertex = geometry->offsetVB;
                    uint64_t offsetIndex = geometry->offsetIB;
                    
                    CopyToBuffer(mesh, *geometry);
                    
                    batchComp->BatchId(batchId);
                    batchComp->OffsetVertex(offsetVertex);
                    batchComp->OffsetIndex(offsetVertex);
                    batchComp->SizeVertex(geometry->offsetVB - offsetVertex);
                    batchComp->SizeIndex(geometry->offsetIB - offsetIndex);
                });
                
                renderComp->Mesh(geometry->mesh, CRenderComponent::kBatchingGroupName);
                renderComp->Material(geometry->material, CRenderComponent::kBatchingGroupName);
                renderComp->Texture(geometry->texture, CRenderComponent::kBatchingGroupName);
                renderComp->Shader(geometry->shader, CRenderComponent::kBatchingGroupName);
            }
            else
            {
                geometry = &m_Batches[batchComp->BatchId()];
            }
            
            batchComp->Transform(transformComp ? transformComp->ResultTransform() : CTransform3Df());
            ApplyTransform(geometry->mesh,
                           batchComp->OffsetVertex(),
                           batchComp->SizeVertex(),
                           batchComp->OldTransform()(),
                           batchComp->Transform()());
        });
    });
    
    ClearDirtyEntities();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

CBatchingSystem::SGeometry CBatchingSystem::CreateNewGeometry(IMaterialPtr material,
                                                               ITexturePtr texture,
                                                               IShaderProgramPtr shader,
                                                               uint64_t elementVertexSize,
                                                               uint64_t elementIndexSize) const
{
    IMeshPtr mesh = GRenderer->CreateMesh();
    assert(mesh);
    IVertexBufferPtr vertexBuffer = GRenderer->CreatVertexBuffer();
    vertexBuffer->Initialize(elementVertexSize);
    
    IIndexBufferPtr indexBuffer = nullptr;
    if (elementIndexSize > 0)
    {
        indexBuffer = GRenderer->CreateIndexBuffer();
        indexBuffer->Initialize(IIndexBuffer::Short); // TODO:
    }
    
    mesh->VertexBuffer(vertexBuffer);
    mesh->IndexBuffer(indexBuffer);
    
    SGeometry geometry;
    geometry.mesh = mesh;
    geometry.material = material;
    geometry.texture = texture;
    geometry.shader = shader;
    
    return geometry;
}

void CBatchingSystem::CopyToBuffer(IMeshPtr srcMesh, SGeometry& geometry)
{
    // Copy vertices to batch buffer
    IVertexBufferPtr dstVertexBuffer = geometry.mesh->VertexBuffer();
    IVertexBufferPtr srcVertexBuffer = srcMesh->VertexBuffer();
    
    if (dstVertexBuffer->Size() < srcVertexBuffer->Size() + geometry.offsetVB)
    {
        dstVertexBuffer->Resize(srcVertexBuffer->Size() + geometry.offsetVB);
    }
    
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

        dstStreamVB.CopyFrom(srcStreamVB, 0, srcVertexBuffer->Size(), geometry.offsetVB);
    });
    
    // Copy indices and recalculate their value in batched buffer
    IIndexBufferPtr dstIndexBuffer = geometry.mesh->IndexBuffer();
    IIndexBufferPtr srcIndexBuffer = srcMesh->IndexBuffer();
    
    if (srcIndexBuffer)
    {
        short index;
        IIndexBuffer::SIndexStream& dstStreamIB = dstIndexBuffer->Lock();
        IIndexBuffer::SIndexStream& srcStreamIB = srcIndexBuffer->Lock();
        uint64_t offsetIndex = geometry.offsetIB;
        for (uint64_t i = 0; i < srcIndexBuffer->Size(); ++i)
        {
            srcStreamIB.GetUnsafe<short>(i, index);
            index += offsetIndex;
            dstStreamIB.SetUnsafe<short>(offsetIndex + i, index);
        }
    }
    
    geometry.offsetVB += srcVertexBuffer->Size();
    geometry.offsetIB += (srcIndexBuffer ? srcIndexBuffer->Size() : 0);
    
    dstVertexBuffer->Unlock(true);
    if (dstIndexBuffer)
    {
        dstIndexBuffer->Unlock(true);
    }
}

void CBatchingSystem::ApplyTransform(IMeshPtr mesh,
                                     uint64_t offset,
                                     uint64_t size,
                                     glm::mat4 oldTransform,
                                     glm::mat4 transform)
{
    IVertexBufferPtr vertexBuffer = mesh->VertexBuffer();
    
    glm::vec3 vec3;
    glm::vec4 vec4;
    
    // Position
    glm::mat4 inverseOldTransform = glm::inverse(oldTransform);
    
    IVertexBuffer::SVertexStream& streamPos = vertexBuffer->Lock(IVertexBuffer::Position);
    for (uint64_t i = 0; i < size; ++i)
    {
        streamPos.GetUnsafe<glm::vec3>(offset + i, vec3);
        vec4 = glm::vec4(vec3, 1.0f);
        vec4 = inverseOldTransform * vec4;
        vec3 = glm::vec3(transform * vec4);
        streamPos.SetUnsafe<glm::vec3>(offset + i, vec3);
    }
    
    // Normal
    glm::mat4 oldNormalTransform(oldTransform);
    oldNormalTransform[0][0] = 1.0f;
    oldNormalTransform[1][1] = 1.0f;
    oldNormalTransform[2][2] = 1.0f;
    glm::mat4 inverseOldNormalTransform = glm::inverse(oldNormalTransform);
    
    glm::mat4 normalTransform(transform);
    normalTransform[0][0] = 1.0f;
    normalTransform[1][1] = 1.0f;
    normalTransform[2][2] = 1.0f;
    
    IVertexBuffer::SVertexStream& streamNormal = vertexBuffer->Lock(IVertexBuffer::Normal);
    for (uint64_t i = 0; i < size; ++i)
    {
        streamNormal.GetUnsafe<glm::vec3>(offset + i, vec3);
        vec4 = glm::vec4(vec3, 1.0f);
        vec4 = inverseOldNormalTransform * vec4;
        vec3 = glm::vec3(normalTransform * vec4);
        streamNormal.SetUnsafe<glm::vec3>(offset + i, vec3);
    }
    
    vertexBuffer->Unlock(true);
}