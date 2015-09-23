//
//  IEntity.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "IEntity.h"
#include "CCESEvent.h"
#include "IEventDispatcher.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

IEntity::IEntity()
: m_HierarchyIndex(0)
{
    
}

IEntity::~IEntity()
{
    
}

void IEntity::Initialize(const std::string& name, const TComponentsList& components)
{
    Name(name);
    
    m_Components.clear();
    std::for_each(components.begin(), components.end(), [&](const TComponentsList::value_type& component)
    {
        AddComponent(component);
    });
}

const std::string& IEntity::Name() const
{
    return m_Name;
}

void IEntity::Name(const std::string& name)
{
    m_Name = name;
}

void IEntity::AddComponent(IComponentPtr component)
{
    if (!component)
    {
        return;
    }
    
    TComponentsList& components = m_Components[component->Id()];
    TComponentsList::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it == components.end())
    {
        components.push_back(component);
        component->Entity(shared_from_this());
    }
}

IComponentPtr IEntity::GetComponent(const std::type_index& id)
{
    TComponentsList& components = m_Components[id];
    if (components.empty())
    {
        return nullptr;
    }
    
    return components[0];
}

void IEntity::RemoveComponent(IComponentPtr component)
{
    if (!component)
    {
        return;
    }
    
    TComponentsList& components = m_Components[component->Id()];
    TComponentsList::const_iterator it = std::find(components.begin(), components.end(), component);
    if (it != components.end())
    {        
        component->Entity(IEntityWeak());
        components.erase(it);
    }
}

void IEntity::RemoveAllComponents(const std::type_index& id)
{
    TComponentsList& components = m_Components[id];
    std::for_each(components.begin(), components.end(), [&](IComponentPtr component)
    {
        RemoveComponent(component);
    });
    components.clear();
}

bool IEntity::HasComponent(const std::type_index& id)
{
    TComponentsList& components = m_Components[id];
    return !components.empty();
}

uint32_t IEntity::ComponentsNum(const std::type_index& id)
{
    TComponentsList& components = m_Components[id];
    return static_cast<uint32_t>(components.size());
}

void IEntity::AddChild(IEntityPtr entity)
{
    if (!entity)
    {
        return;
    }
    
    IEntityPtr prevParent = entity->Parent().lock();
    assert(prevParent != entity);
    if (prevParent == entity &&
        prevParent != nullptr)
    {
        return;
    }
    
    if (prevParent)
    {
        prevParent->RemoveChild(entity);
    }
    
    entity->Parent(shared_from_this());
    entity->HierarchyIndex(HierarchyIndex() + 1);
    m_Entities.push_back(entity);
    
    CCESEventPtr addedEvent(new CCESEvent(entity, CCESEvent::Added));
    Dispatcher()->DispatchEvent(addedEvent);
    
    CCESEventPtr changedEvent(new CCESEvent(entity, CCESEvent::Changed));
    Dispatcher()->DispatchEvent(changedEvent);
}

void IEntity::RemoveChild(IEntityPtr entity)
{
    IEntity::TEntities::const_iterator it = std::find(m_Entities.begin(), m_Entities.end(), entity);
    if (it != m_Entities.end())
    {
        (*it)->Parent(IEntityWeak());
        (*it)->HierarchyIndex(0);
        m_Entities.erase(it);
    }
    
    CCESEventPtr event(new CCESEvent(entity, CCESEvent::Removed));
    Dispatcher()->DispatchEvent(event);
}

const IEntity::TEntities& IEntity::Childs() const
{
    return m_Entities;
}

IEntityWeak IEntity::Parent() const
{
    return m_Parent;
}

uint32_t IEntity::HierarchyIndex() const
{
    return m_HierarchyIndex;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void IEntity::Parent(IEntityWeak parent)
{
    m_Parent = parent;
}

void IEntity::HierarchyIndex(uint32_t hierarchyIndex)
{
    m_HierarchyIndex = hierarchyIndex;
    const TEntities& childs = Childs();
    std::for_each(childs.begin(), childs.end(), [&](IEntityPtr entity)
    {
        entity->HierarchyIndex(HierarchyIndex() + 1);
    });
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
