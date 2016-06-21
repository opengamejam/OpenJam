//
//  ISystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "ISystem.h"
#include "IEntity.h"
#include "IComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

ISystem::ISystem()
{
}

ISystem::~ISystem()
{
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

bool ISystem::IsComponentRegistered(typeid_t id)
{
    return (m_RegisteredComponents.find(id) != m_RegisteredComponents.end());
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

bool ISystem::HaveSupportedComponents(IEntityPtr entity)
{
    if (!entity)
    {
        return false;
    }
    
    bool found = false;
    const ISystem::TComponentIds& componentIds = RegisteredComponents();
    std::all_of(componentIds.begin(), componentIds.end(), [&](typeid_t id)
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

void ISystem::OnChangedComponent(IComponentPtr component)
{
    MarkDirtyEntity(component->Entity().lock());
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
