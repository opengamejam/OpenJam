//
//  ISystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "ISystem.h"
#include "IEntity.h"
#include "IEventDispatcher.hpp"
#include "CCESEvent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

ISystem::ISystem()
{
    Dispatcher()->RegisterEventHandler<CCESEvent>(std::bind(&ISystem::OnComponentChanged, this,
                                                                  std::placeholders::_1));
}

ISystem::~ISystem()
{
    Dispatcher()->UnregisterEventHandler<CCESEvent>(std::bind(&ISystem::OnComponentChanged, this,
                                                                    std::placeholders::_1));
}

void ISystem::AddEntity(IEntityPtr entity)
{
    m_Entities.insert(entity);
}

void ISystem::RemoveEntity(IEntityPtr entity)
{
    m_Entities.erase(entity);
}

const ISystem::TEntities& ISystem::Entities() const
{
    return m_Entities;
}

bool ISystem::IsEntityAdded(IEntityPtr entity)
{
    const ISystem::TEntities& entities = Entities();
    ISystem::TEntities::const_iterator it = std::find(entities.begin(), entities.end(), entity);

    return (it != entities.end());
}

const ISystem::TComponentIds& ISystem::RegisteredComponents()
{
    return m_RegisteredComponents;
}

bool ISystem::IsComponentRegistered(const std::type_index& id)
{
    return (m_RegisteredComponents.find(id) != m_RegisteredComponents.end());
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void ISystem::RegisterComponent(const std::type_index& id)
{
    m_RegisteredComponents.insert(id);
}

void ISystem::UnregisterComponent(const std::type_index& id)
{
    m_RegisteredComponents.erase(id);
}

bool ISystem::HaveSupportedComponents(IEntityPtr entity)
{
    if (!entity)
    {
        return false;
    }
    
    bool found = false;
    const ISystem::TComponentIds& componentIds = RegisteredComponents();
    std::all_of(componentIds.begin(), componentIds.end(), [&](const std::type_index& id)
    {
        found = entity->HasComponent(id);
        return !found;
    });
    
    return found;
}

void ISystem::MarkDirtyEntity(IEntityPtr entity)
{
    m_DirtyEntities.insert(entity);
}

void ISystem::UnmarkDirtyEntity(IEntityPtr entity)
{
    m_DirtyEntities.erase(entity);
}

const ISystem::TEntities& ISystem::DirtyEntities() const
{
    return m_DirtyEntities;
}

void ISystem::ClearDirtyEntities()
{
    m_DirtyEntities.clear();
}

void ISystem::OnAddedEntity(IEntityPtr entity)
{
    if (!IsEntityAdded(entity) &&
        HaveSupportedComponents(entity))
    {
        AddEntity(entity);
    }
}

void ISystem::OnChangedEntity(IEntityPtr entity)
{
    MarkDirtyEntity(entity);
}

void ISystem::OnRemovedEntity(IEntityPtr entity)
{
    if (IsEntityAdded(entity))
    {
        RemoveEntity(entity);
    }
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

bool ISystem::OnComponentChanged(IEventPtr event)
{
    CCESEventPtr componentEvent = std::static_pointer_cast<CCESEvent>(event);
    assert(componentEvent);
    
    IEntityPtr entity = componentEvent->Entity().lock();
    if (!entity)
    {
        return false; // Stop propagation for non-valid entities
    }
    
    CCESEvent::ActionType action = componentEvent->Action();
    switch (action)
    {
        case CCESEvent::Added:
        {
            OnAddedEntity(entity);
        }
        break;
        case CCESEvent::Changed:
        {
            IComponentPtr component = componentEvent->Component().lock();
            if (component && IsComponentRegistered(component->Id()))
            {
                OnChangedEntity(entity);
            }
        }
        break;
        case CCESEvent::Removed:
        {
            OnRemovedEntity(entity);
        }
        break;
    };
    
    return true;
}