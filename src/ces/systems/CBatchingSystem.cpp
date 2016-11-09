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

CBatchingSystem::CBatchingSystem(IRendererPtr renderer)
    : m_Renderer(renderer)
{
    assert(m_Renderer);
    RegisterComponent<CBatchComponent>();
    RegisterComponent<CTransformationComponent>();
}

CBatchingSystem::~CBatchingSystem()
{
}

void CBatchingSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity) {
        CTransformationComponentPtr transformComp = entity->Get<CTransformationComponent>();
        CBatchComponentPtr batchComp = entity->Get<CBatchComponent>();
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComp) {
            if (!batchComp || !renderComp->Batchable()) {
                return;
            }

            SGeometry* geometry = nullptr;
            if (batchComp->BatchId() == 0) {
                // Copy to buffer
                const std::set<std::string>& groups = renderComp->Groups();
                std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName) {
                    if (groupName == CRenderComponent::kBatchingGroupName || !renderComp->Visible(groupName)) {
                        return;
                    }

                    IMeshPtr mesh = renderComp->Mesh(groupName);
                    IMaterialPtr material = renderComp->Material(groupName);
                    ITexturePtr texture = renderComp->Texture(groupName);
                    IShaderProgramPtr shader = renderComp->Shader(groupName);

                    const std::string& matId = (material ? material->Hash() : "");
                    const std::string& texId = (texture ? texture->Hash() : "");
                    std::tuple<std::string, std::string> container(matId, texId); // TODO: shaderId, cameraId
                    uint64_t batchId = hash_tuple(container);

                    TBatchCache::iterator it = m_Batches.find(batchId);
                    if (it == m_Batches.end()) {
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
            } else {
                geometry = &m_Batches[batchComp->BatchId()];
            }

            batchComp->Transform(transformComp ? transformComp->AbsoluteTransformation() : CTransform3Df());
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
    IMeshPtr mesh = m_Renderer->CreateMesh();
    assert(mesh);
    IVertexBufferPtr vertexBuffer = m_Renderer->CreateVertexBuffer();
    vertexBuffer->Initialize(elementVertexSize);

    IIndexBufferPtr indexBuffer = nullptr;
    if (elementIndexSize > 0) {
        indexBuffer = m_Renderer->CreateIndexBuffer();
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

    if (dstVertexBuffer->Size() < srcVertexBuffer->Size() + geometry.offsetVB) {
        dstVertexBuffer->Resize(srcVertexBuffer->Size() + geometry.offsetVB);
    }

    const IVertexBuffer::TVertexStreamMap& srcStreamsVB = srcVertexBuffer->VertexStreams();
    std::for_each(srcStreamsVB.begin(), srcStreamsVB.end(), [&](const IVertexBuffer::TVertexStreamMap::value_type& value) {
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

    if (srcIndexBuffer) {
        short index;
        IIndexBuffer::SIndexStream& dstStreamIB = dstIndexBuffer->Lock();
        IIndexBuffer::SIndexStream& srcStreamIB = srcIndexBuffer->Lock();
        uint64_t offsetIndex = geometry.offsetIB;
        for (uint64_t i = 0; i < srcIndexBuffer->Size(); ++i) {
            srcStreamIB.GetUnsafe<short>(i, index);
            index += offsetIndex;
            dstStreamIB.SetUnsafe<short>(offsetIndex + i, index);
        }
    }

    geometry.offsetVB += srcVertexBuffer->Size();
    geometry.offsetIB += (srcIndexBuffer ? srcIndexBuffer->Size() : 0);

    dstVertexBuffer->Unlock(true);
    if (dstIndexBuffer) {
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
    for (uint64_t i = 0; i < size; ++i) {
        streamPos.GetUnsafe<glm::vec3>(offset + i, vec3);
        vec4 = inverseOldTransform * glm::vec4(vec3, 1.0f);
        vec3 = glm::vec3(transform * vec4);
        streamPos.SetUnsafe<glm::vec3>(offset + i, vec3);
    }

    // Normal
    glm::mat4 inverseOldNormalTransform = glm::transpose(oldTransform);
    glm::mat4 oldNormalTransform = glm::inverse(oldNormalTransform);
    glm::mat4 normalTransform = glm::transpose(glm::inverse(transform));

    IVertexBuffer::SVertexStream& streamNormal = vertexBuffer->Lock(IVertexBuffer::Normal);
    for (uint64_t i = 0; i < size; ++i) {
        streamNormal.GetUnsafe<glm::vec3>(offset + i, vec3);
        vec4 = inverseOldNormalTransform * glm::vec4(vec3, 1.0f);
        vec3 = glm::vec3(normalTransform * vec4);
        streamNormal.SetUnsafe<glm::vec3>(offset + i, vec3);
    }

    vertexBuffer->Unlock(true);
}
