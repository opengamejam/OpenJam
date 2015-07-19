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
}

void CRenderSystem::Draw(ICameraPtr camera)
{
    const ISystem::TEntitiesList& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            if (!renderComponent->IsValid())
            {
                return;
            }
            
            if (!renderComponent->Batchable())
            {
                IMeshPtr mesh = renderComponent->Mesh();
                IMaterialPtr material = renderComponent->Material();
                ITexturePtr texture = renderComponent->Texture();
                IShaderProgramPtr shader = renderComponent->Shader();
                
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
            }
        });
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
