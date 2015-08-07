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

#include "RenderGlobal.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderSystem::CRenderSystem()
{
    RegisterComponent(ComponentId<CRenderComponent>());
}

CRenderSystem::~CRenderSystem()
{
    
}

void CRenderSystem::Update(unsigned long dt)
{
    const ISystem::TEntitiesList& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            if (!IsEntityAdded(entity))
            {
                return;
            }
            
            bool isPartOfBatch = renderComponent->IsBatchIndexValid();
            if (renderComponent->Visible())
            {
                if (!isPartOfBatch)
                {
                    // Add to batch
                }
            }
            else
            {
                if (isPartOfBatch)
                {
                    // Remove from batch
                }
            }
        });
    });
    
    ClearDirtyEntities();
    m_ProccededRenderTargets.clear();
}

void CRenderSystem::Draw(ICameraPtr camera)
{
    if (!camera)
    {
        return;
    }
    
    IRenderTargetPtr currentRenderTarget = camera->RenderTarget();
    if (!currentRenderTarget)
    {
        return;
    }
    
    currentRenderTarget->Bind();
    if (m_ProccededRenderTargets.find(currentRenderTarget) == m_ProccededRenderTargets.end())
    {
        currentRenderTarget->Clear();
        m_ProccededRenderTargets.insert(currentRenderTarget);
    }
    
    const ISystem::TEntitiesList& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            unsigned int cameraId = camera->Id();
            if (!renderComponent->IsValid() ||
                !renderComponent->HasCameraId(cameraId))
            {
                return;
            }
            
            if (!renderComponent->Batchable())
            {
                const std::set<std::string>& groups = renderComponent->Groups();
                std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
                {
                    IMeshPtr mesh = renderComponent->Mesh(groupName);
                    IMaterialPtr material = renderComponent->Material(groupName);
                    ITexturePtr texture = renderComponent->Texture(groupName);
                    IShaderProgramPtr shader = renderComponent->Shader(groupName);
                    
                    if (!shader || !material || !mesh)
                    {
                        return;
                    }
                    
                    shader->Bind();
                    material->Bind();
                    if (texture)
                    {
                        texture->Bind();
                    }
                    mesh->Bind();
                    
                    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
                    {
                        CTransform3Df resultTransform = transformComponent->ResultTransform();
                        material->BindUniformMatrix4x4f(shader->ModelMatrix(), resultTransform());
                    });
                    material->BindUniformMatrix4x4f(shader->ProjectionMatrix(), camera->ProjectionMatrix());
                    material->UpdateUniforms();
                    
                    GRenderer->Draw(mesh, material, shader);
                    
                    mesh->Unbind();
                    if (texture)
                    {
                        texture->Unbind();
                    }
                    material->Unbind();
                    shader->Unbind();
                });
            }
        });
    });
    
    currentRenderTarget->Unbind();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
