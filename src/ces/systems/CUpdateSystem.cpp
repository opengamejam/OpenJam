//
//  CUpdateSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CUpdateSystem.h"
#include "CUpdateComponent.h"
#include "IEntity.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CUpdateSystem::CUpdateSystem()
{
    RegisterComponent<CUpdateComponent>();
}

CUpdateSystem::~CUpdateSystem()
{

}

void CUpdateSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = Entities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity)
    {
        entity->Get<CUpdateComponent>([dt](CUpdateComponentPtr updateComponent)
        {
            updateComponent->Update(dt);
        });
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
