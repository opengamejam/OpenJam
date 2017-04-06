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

/*template<typename ComponentType>
CSystemBase<ComponentType>::CSystemBase()
: m_RequiredComponent(CTypeId<ComponentType>::Id())
{
    ComponentType::OnAddedSignal += std::bind(&CSystemBase::OnAddedComponent, this, std::placeholders::_1);
    ComponentType::OnRemovedSignal += std::bind(&CSystemBase::OnRemovedComponent, this, std::placeholders::_1);
    ComponentType::OnChangedSignal += std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
}

template<typename ComponentType>
CSystemBase<ComponentType>::~CSystemBase()
{
    ComponentType::OnAddedSignal -= std::bind(&CSystemBase::OnAddedComponent, this, std::placeholders::_1);
    ComponentType::OnRemovedSignal -= std::bind(&CSystemBase::OnRemovedComponent, this, std::placeholders::_1);
    ComponentType::OnChangedSignal -= std::bind(&CSystemBase::OnChangedComponent, this, std::placeholders::_1);
}

template<typename ComponentType>
const std::set<IComponentPtr>& CSystemBase<ComponentType>::Components() const
{
    return m_Components;
}

template<typename ComponentType>
typeid_t CSystemBase<ComponentType>::RequiredComponent() const
{
    return m_RequiredComponent;
}

template<typename ComponentType>
bool CSystemBase<ComponentType>::IsSupportComponent(typeid_t id)
{
    return id == m_RequiredComponent;
}*/

// *****************************************************************************
// Protected Methods
// *****************************************************************************

/*template<typename ComponentType>
const std::set<IComponentPtr>& CSystemBase<ComponentType>::DirtyComponents() const
{
    return m_DirtyComponents;
}

template<typename ComponentType>
void CSystemBase<ComponentType>::ClearDirtyComponents()
{
    m_DirtyComponents.clear();
}

template<typename ComponentType>
void CSystemBase<ComponentType>::OnAddedComponent(IComponentPtr component)
{
    if (IsSupportComponent(component->GetId())) {
        m_Components.insert(component);
    }
}

template<typename ComponentType>
void CSystemBase<ComponentType>::OnChangedComponent(IComponentPtr component)
{
    if (IsSupportComponent(component->GetId())) {
        m_DirtyComponents.insert(component);
    }
}

template<typename ComponentType>
void CSystemBase<ComponentType>::OnRemovedComponent(IComponentPtr component)
{
    if (IsSupportComponent(component->GetId())) {
        m_Components.erase(component);
    }
}*/

// *****************************************************************************
// Private Methods
// *****************************************************************************
