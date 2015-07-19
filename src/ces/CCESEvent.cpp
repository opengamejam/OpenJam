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

IEntityWeak CCESEvent::Entity() const
{
    return m_Entity;
}

CCESEvent::ActionType CCESEvent::Action() const
{
    return m_Action;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************