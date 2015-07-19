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

const ISystem::TEntitiesList& ISystem::Entities() const
{
    return m_Entities;
}

bool ISystem::IsEntityAdded(IEntityPtr entity)
{
    const ISystem::TEntitiesList& entities = Entities();
    ISystem::TEntitiesList::const_iterator it = std::find(entities.begin(), entities.end(), entity);

    return (it != entities.end());
}

const ISystem::TComponentIdsList& ISystem::RegisteredComponents()
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

bool ISystem::IsHasSupportedComponents(IEntityPtr entity)
{
    if (!entity)
    {
        return false;
    }
    
    bool found = false;
    const ISystem::TComponentIdsList& componentIds = RegisteredComponents();
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

const ISystem::TEntitiesList& ISystem::DirtyEntities() const
{
    return m_DirtyEntities;
}

void ISystem::ClearDirtyEntities()
{
    m_DirtyEntities.clear();
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
            if (!IsEntityAdded(entity))
            {
                AddEntity(entity);
            }
        }
        break;
        case CCESEvent::Changed:
        {
            if (IsHasSupportedComponents(entity))
            {
                MarkDirtyEntity(entity);
            }
        }
        break;
        case CCESEvent::Removed:
        {
            if (IsEntityAdded(entity))
            {
                RemoveEntity(entity);
            }
        }
        break;
    };
    
    return true;
}