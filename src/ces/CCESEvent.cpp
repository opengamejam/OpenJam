//
//  CCESEvent.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CCESEvent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CCESEvent::CCESEvent(IEntityWeak entity, ActionType action)
: m_Entity(entity)
, m_Action(action)
{

}

CCESEvent::~CCESEvent()
{

}

const std::string& CCESEvent::Id() const
{
    return m_Id;
}

IEntityWeak CCESEvent::Entity() const
{
    return m_Entity;
}

CCESEvent::ActionType CCESEvent::Action() const
{
    return m_Action;
}

IComponentWeak CCESEvent::Component() const
{
    return m_Component;
}

void CCESEvent::Component(IComponentWeak component)
{
    m_Component = component;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************