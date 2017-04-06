//
//  CEventSystem.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/16/16.
//
//

#include "CEventSystem.h"
#include "CEventComponent.h"
#include "IEntity.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CEventSystem::CEventSystem()
{
}

CEventSystem::~CEventSystem()
{
}

void CEventSystem::DispatchEvent(IEventPtr event)
{
    AllOfComponents([&](CEventComponentPtr eventComponent) {
        bool stopPropagation = eventComponent->DispatchEvent(event);
        return !stopPropagation;
    });
}

void CEventSystem::Update(unsigned long dt)
{
    ForEachDirtyComponents([&](CEventComponentPtr eventComponent) {
        IEventPtr event = eventComponent->Pop();
        while (event) {
            DispatchEvent(event);
            event = eventComponent->Pop();
        }
    });
    
    ClearDirtyComponents();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
