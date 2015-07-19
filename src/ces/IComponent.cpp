//
//  IComponent.cpp
//  Arkanoid
//
//  Created by yev on 2/6/15.
//
//

#include "IComponent.h"
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

IComponent::IComponent(const std::type_index& id)
: m_CachedId(id)
{

}

const std::type_index& IComponent::Id() const
{
    return m_CachedId;
}

void IComponent::Entity(IEntityWeak entityWeak)
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
        CCESEventPtr event(new CCESEvent(prevEntity, CCESEvent::Changed));
        Dispatcher()->DispatchEvent(event);
    }
    
    m_Entity = entityWeak;
    
    if (newEntity)
    {
        newEntity->AddComponent(thisPtr);
        CCESEventPtr event(new CCESEvent(newEntity, CCESEvent::Changed));
        Dispatcher()->DispatchEvent(event);
    }
}

IEntityWeak IComponent::Entity() const
{
    return m_Entity;
}

void IComponent::Dirty()
{
    CCESEventPtr event(new CCESEvent(Entity(), CCESEvent::Changed));
    Dispatcher()->DispatchEvent(event);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************