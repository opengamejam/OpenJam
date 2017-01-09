//
//  CTransfromationSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CTransformationSystem.h"
#include "CTransformationComponent.h"
#include "CRenderComponent.h"
#include "IMaterial.h"
#include "IShaderProgram.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTransfromationSystem::CTransfromationSystem()
{
    RegisterComponent<CTransformationComponent>();
}

CTransfromationSystem::~CTransfromationSystem()
{
}

void CTransfromationSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity) {
        if (!IsEntityAdded(entity)) {
            return;
        }

        UpdateTransformsRecursively(entity);
    });

    ClearDirtyEntities();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CTransfromationSystem::UpdateTransformsRecursively(IEntityPtr entity)
{
    CTransform3Df absoluteTransform;
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent) {
        transformComponent->UpdateAbsoluteTransform();
        absoluteTransform = transformComponent->AbsoluteTransformation();
        
        const IEntity::TEntities& childs = entity->Children();
        std::for_each(childs.begin(), childs.end(), [&](IEntityPtr child) {
            child->Get<CTransformationComponent>([&](CTransformationComponentPtr childTransformComponent) {
                childTransformComponent->AddTransform(CTransformationComponent::Parent,
                                                      transformComponent->ChildrenTransformation());
            });
            UpdateTransformsRecursively(child);
        });
    });
    
    entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent) {
        IShaderProgramPtr shader = renderComponent->Shader();
        if (shader) {
            if (renderComponent->Batchable()) {
                absoluteTransform = CTransform3Df();
            }
            
            shader->BindUniformMatrix4x4f("MainModelMatrix", absoluteTransform());
            renderComponent->Shader(shader);
        }
    });
}
