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
    RegisterComponent<CEventComponent>();
}

CEventSystem::~CEventSystem()
{
}

void CEventSystem::DispatchEvent(IEventPtr event)
{
    const ISystem::TEntities& entities = Entities();
    std::all_of(entities.begin(), entities.end(), [&](IEntityPtr entity) {
        bool stopPropagation = false;
        entity->Get<CEventComponent>([&](CEventComponentPtr eventComponent) {
            stopPropagation = eventComponent->DispatchEvent(event);
        });

        return !stopPropagation;
    });
}

void CEventSystem::Update(unsigned long dt)
{
    const ISystem::TEntities& entities = DirtyEntities();
    std::for_each(entities.begin(), entities.end(), [&](IEntityPtr entity) {
        entity->Get<CEventComponent>([&](CEventComponentPtr eventComponent) {
            IEventPtr event = eventComponent->Pop();
            while (event) {
                DispatchEvent(event);

                event = eventComponent->Pop();
            }
        });
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CEventSystem::OnChangedComponent(IComponentPtr component)
{
    CSystemBase::OnChangedComponent(component);
    if (!IsComponentRegistered(component->GetId())) {
        return;
    }

    CEventComponentPtr eventComponent = std::static_pointer_cast<CEventComponent>(component);
    while (eventComponent->DispatchCount()) {
        IEventPtr event = eventComponent->Pop();
        DispatchEvent(event);
    }
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
