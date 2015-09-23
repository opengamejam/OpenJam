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
    RegisterComponent(ComponentId<CTransformationComponent>());
}

CTransfromationSystem::~CTransfromationSystem()
{

}

void CTransfromationSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        if (!IsEntityAdded(entity))
        {
            return;
        }
        
        CTransform3Df childTransform;
        IEntityPtr parent = entity->Parent().lock();
        if (!parent)
        {
        	return;
        }

        parent->Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
        {
            const CTransformationComponent::TTransformsList& transforms = transformComponent->Transforms();
            std::for_each(transforms.begin(), transforms.end(),
                          [&](const std::pair<int, CTransformationComponent::STransformProps>& element)
            {
                const CTransformationComponent::STransformProps& transformProps = element.second;
                if (transformProps.isAffectsOnChilds)
                {
                    childTransform += transformProps.transform;
                }
            });
        });
        
        UpdateTransformsRecursively(entity, childTransform);
    });
    
    ClearDirtyEntities();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CTransfromationSystem::UpdateTransformsRecursively(IEntityPtr entity,
                                                        const CTransform3Df& parentTransform)
{
    entity->Get<CTransformationComponent>([&](CTransformationComponentPtr transformComponent)
    {
        // Calculate summary transform
        CTransform3Df resultTransform = parentTransform;
        CTransform3Df childTransform = parentTransform;
        const CTransformationComponent::TTransformsList& transforms = transformComponent->Transforms();
        
        std::for_each(transforms.begin(), transforms.end(),
                      [&](const std::pair<int, CTransformationComponent::STransformProps>& element)
        {
            const CTransformationComponent::STransformProps& transformProps = element.second;
            if (transformProps.isAffectsOnChilds)
            {
                childTransform += transformProps.transform;
            }
            resultTransform += transformProps.transform;
        });
        transformComponent->ResultTransform(resultTransform);
        
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent)
        {
            IShaderProgramPtr shader = renderComponent->Shader();
            if (shader)
            {
                shader->BindUniformMatrix4x4f("MainModelMatrix", resultTransform());
                renderComponent->Shader(shader);
            }
        });
        
        // Update transform in childrens
        const IEntity::TEntities& childs = entity->Childs();
        std::for_each(childs.begin(), childs.end(), [&](IEntityPtr entity)
        {
            UpdateTransformsRecursively(entity, childTransform);
        });
    });
}
