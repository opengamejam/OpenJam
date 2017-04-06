
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
}

CUpdateSystem::~CUpdateSystem()
{
}

void CUpdateSystem::Update(unsigned long dt)
{
    ForEachDirtyComponents([&](CUpdateComponentPtr updateComponent) {
        updateComponent->Update(dt);
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

