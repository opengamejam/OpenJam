//
//  IComponent.cpp
//  Arkanoid
//
//  Created by yev on 2/6/15.
//
//

#include "IComponent.h"
#include "IEntity.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

CSignal<void, IEntityPtr> IComponent::OnAddedSignal;
CSignal<void, IEntityPtr> IComponent::OnRemovedSignal;
CSignal<void, IComponentPtr> IComponent::OnChangedSignal;

// *****************************************************************************
// Public Methods
// *****************************************************************************

void CComponentBase::Entity(IEntityWeak entityWeak)
{
    IEntityPtr prevEntity = m_Entity.lock();
    IEntityPtr newEntity = entityWeak.lock();
    if (newEntity == prevEntity)
    {
        return;
    }
    
    IComponentPtr thisPtr = shared_from_this();
    
    if (prevEntity)
    {
        prevEntity->RemoveComponent(thisPtr);
    }
    
    m_Entity = newEntity;
    
    if (newEntity)
    {
        newEntity->AddComponent(thisPtr);
    }
    
    Dirty();
}

IEntityWeak CComponentBase::Entity() const
{
    return m_Entity;
}

void CComponentBase::Dirty()
{
    IComponentPtr thisPtr = shared_from_this();
    emit OnChangedSignal(thisPtr);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************