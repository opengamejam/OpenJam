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

CSystemBase::CSystemBase()
{
}

CSystemBase::~CSystemBase()
{
}

void CSystemBase::AddEntity(IEntityPtr entity)
{
    m_Entities.insert(entity);
}

void CSystemBase::RemoveEntity(IEntityPtr entity)
{
    m_Entities.erase(entity);
}

const ISystem::TEntities& CSystemBase::Entities() const
{
    return m_Entities;
}

bool CSystemBase::IsEntityAdded(IEntityPtr entity)
{
    const ISystem::TEntities& entities = Entities();
    ISystem::TEntities::const_iterator it = std::find(entities.begin(), entities.end(), entity);

    return (it != entities.end());
}

const ISystem::TComponentIds& CSystemBase::RegisteredComponents()
{
    return m_RegisteredComponents;
}

bool CSystemBase::IsComponentRegistered(typeid_t id)
{
    return (m_RegisteredComponents.find(id) != m_RegisteredComponents.end());
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

bool CSystemBase::HaveSupportedComponents(IEntityPtr entity)
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

void CSystemBase::MarkDirtyEntity(IEntityPtr entity)
{
    m_DirtyEntities.insert(entity);
}

const ISystem::TEntities& CSystemBase::DirtyEntities() const
{
    return m_DirtyEntities;
}

void CSystemBase::ClearDirtyEntities()
{
    m_DirtyEntities.clear();
}

void CSystemBase::OnAddedEntity(IEntityPtr entity)
{
    if (!IsEntityAdded(entity) &&
        HaveSupportedComponents(entity))
    {
        AddEntity(entity);
    }
}

void CSystemBase::OnChangedComponent(IComponentPtr component)
{
    MarkDirtyEntity(component->Entity().lock());
}

void CSystemBase::OnRemovedEntity(IEntityPtr entity)
{
    if (IsEntityAdded(entity))
    {
        RemoveEntity(entity);
    }
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
