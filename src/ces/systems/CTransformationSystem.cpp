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
#include "IEntity.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTransfromationSystem::CTransfromationSystem()
{
}

CTransfromationSystem::~CTransfromationSystem()
{
}

void CTransfromationSystem::Update(unsigned long dt)
{
    ForEachDirtyComponents([&](CTransformationComponentPtr transformationComponent) {
        UpdateTransformsRecursively(transformationComponent);
    });

    ClearDirtyComponents();
}

void CTransfromationSystem::UpdateTransformsRecursively(CTransformationComponentPtr transformationComponent)
{
    transformationComponent->UpdateAbsoluteTransform();
    CTransform3Df absoluteTransform = transformationComponent->AbsoluteTransformation();
    
    IEntityPtr entity = transformationComponent->Entity();
    if (!entity) { // TODO: check if need this condition
        return;
    }
    
    // Assign model transformation to shader
    entity->Get<CRenderComponent, true>([&](CRenderComponentPtr renderComponent) {
        IShaderProgramPtr shader = renderComponent->Shader();
        if (!shader) {
            return;
        }
        
        if (renderComponent->Batchable()) {
            absoluteTransform = CTransform3Df();
        }
        
        shader->BindUniformMatrix4x4f("MainModelMatrix", absoluteTransform());
        renderComponent->Shader(shader);
    });
    
    // Update children recursively
    const IEntity::TEntitiesList& childs = entity->Children();
    std::for_each(childs.begin(), childs.end(), [&](IEntityPtr child) {
        child->Get<CTransformationComponent>([&](CTransformationComponentPtr childTransformComponent) {
            childTransformComponent->AddTransform(CTransformationComponent::Parent,
                                                  transformationComponent->ChildrenTransformation());
            UpdateTransformsRecursively(childTransformComponent);
        });
        // Some entities can be without transformation components, so children of these entities
        // will have parent transform (0, 0, 0)
    });
}
