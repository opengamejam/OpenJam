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

CSignal<void, IComponentPtr> IComponent::OnAddedSignal;
CSignal<void, IComponentPtr> IComponent::OnRemovedSignal;
CSignal<void, IComponentPtr> IComponent::OnChangedSignal;

// *****************************************************************************
// Public Methods
// *****************************************************************************

void CComponentBase::Entity(IEntityPtr entity)
{
    IEntityPtr prevEntity = Entity();
    if (entity == prevEntity) {
        return;
    }

    IComponentPtr thisPtr = shared_from_this();

    if (prevEntity) {
        prevEntity->RemoveComponent(thisPtr);
    }

    m_Entity = entity;

    if (entity) {
        entity->AddComponent(thisPtr);
    }

    Dirty();
}

IEntityPtr CComponentBase::Entity() const
{
    return m_Entity.lock();
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
