//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CRenderSystem.h"
#include "IEntity.h"
#include "ICamera.h"
#include "CRenderComponent.h"
#include "CTransformationComponent.h"
#include "CBatchComponent.h"
#include "RenderGlobal.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderSystem::CRenderSystem(IRendererPtr renderer)
: m_Renderer(renderer)
{
    assert(m_Renderer);
}

CRenderSystem::~CRenderSystem()
{
}

void CRenderSystem::Update(unsigned long dt)
{
    ClearDirtyComponents();
    m_ProccededFrameBuffers.clear();
}

void CRenderSystem::Draw(ICameraPtr camera, IEntityPtr root)
{
    if (!camera) {
        return;
    }

    IFrameBufferPtr frameBuffer = camera->FrameBuffer();
    if (!frameBuffer) {
        return;
    }

    // Clear render target before drawing
    frameBuffer->Bind();
    if (m_ProccededFrameBuffers.find(frameBuffer) == m_ProccededFrameBuffers.end()) {
        frameBuffer->Clear();
        m_ProccededFrameBuffers.insert(frameBuffer);
    }

    glm::mat4 projectionMatrix = camera->ProjectionMatrix();
    glm::mat4 viewMatrix(1.0);
    camera->Get<CTransformationComponent>([&](CTransformationComponentPtr viewTransform) {
        viewMatrix = viewTransform->AbsoluteTransformation()();
    });

    // Draw
    root->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent) {
        if (renderComponent->Batchable()) {
            IMeshPtr mesh = renderComponent->Mesh(CRenderComponent::kBatchingGroupName);
            if (m_ProccededBatches.find(mesh->GetUid()) != m_ProccededBatches.end()) {
                return;
            }

            JAM_LOG("CRenderSystem::Draw - Draw batch: %s\n", renderComponent->Entity()->Name().c_str());
            DrawGroup(renderComponent, CRenderComponent::kBatchingGroupName, projectionMatrix, viewMatrix, glm::mat4(1.0));
            m_ProccededBatches.insert(mesh->GetUid());
        } else {
            IEntityPtr entity = renderComponent->Entity();
            glm::mat4 modelMatrix(1.0);
            if (entity) {
                entity->Get<CTransformationComponent>([&](CTransformationComponentPtr modelTransform) {
                    modelMatrix = modelTransform->AbsoluteTransformation()();
                });
            }

            const std::set<std::string>& groups = renderComponent->Groups();
            std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName) {
                if (groupName == CRenderComponent::kBatchingGroupName || !renderComponent->Visible(groupName)) {
                    return;
                }
                
                JAM_LOG("CRenderSystem::Draw - Draw object: %s\n", renderComponent->Entity()->Name().c_str());
                DrawGroup(renderComponent, groupName, projectionMatrix, viewMatrix, modelMatrix);
            });
        }
    });
    
    const IEntity::TEntitiesList& entities = root->Children();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        Draw(camera, entity);
    });

    frameBuffer->Unbind();
    m_ProccededBatches.clear();
}

void CRenderSystem::DrawGroup(CRenderComponentPtr renderComponent,
    const std::string& groupName,
    const glm::mat4& projectionMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& modelMatrix) const
{
    IMeshPtr mesh = renderComponent->Mesh(groupName);
    IMaterialPtr material = renderComponent->Material(groupName);
    ITexturePtr texture = renderComponent->Texture(groupName);
    IShaderProgramPtr shader = renderComponent->Shader(groupName);
    if (!shader || !material || !mesh) {
        return;
    }

    glm::mat4 normalMat = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
    shader->BindUniformMatrix4x4f("MainNormalMatrix", normalMat);
    shader->BindUniformMatrix4x4f("MainProjectionMatrix", projectionMatrix);
    shader->BindUniformMatrix4x4f("MainViewMatrix", viewMatrix);

    Draw(mesh, material, texture, shader);
}

void CRenderSystem::Draw(IMeshPtr mesh, IMaterialPtr material, ITexturePtr texture, IShaderProgramPtr shader) const
{
    shader->Bind();
    shader->UpdateUniforms();
    material->Bind();
    if (texture) {
        texture->Bind();
    }
    mesh->Bind();

    m_Renderer->Draw(mesh, material, shader);

    mesh->Unbind();
    if (texture) {
        texture->Unbind();
    }
    material->Unbind();
    shader->Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
