//
//  CEventComponent.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/14/16.
//
//

#include "CEventComponent.h"
#include "CFunction.h"
#include "CMathFunc.hpp"
#include "IEvent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CEventComponent::CEventComponent()
: m_DispatchCount(0)
{

}

CEventComponent::~CEventComponent()
{

}

void CEventComponent::RegisterHandler(typeid_t eventId, const TEventHandler& eventHandler)
{    
    m_Handlers[eventId] = eventHandler;
}

void CEventComponent::UnregisterHandler(typeid_t eventId, const TEventHandler& eventHandler)
{
    static CFunction<bool, IEventPtr> func;
    
    TEvenetHandlersMap::const_iterator it = m_Handlers.find(eventId);
    if (it == m_Handlers.end())
    {
        return;
    }
    
    const TEventHandler& registeredHandler = it->second;
    if (func.IsEqual(registeredHandler, eventHandler))
    {
        m_Handlers.erase(eventId);
    }
    else
    {
        // TODO: Log
    }
}

void CEventComponent::AddEvent(IEventPtr event, bool fire)
{
    if (fire)
    {
        m_Events.insert(m_Events.begin(), event);
        m_DispatchCount++;
    }
    else
    {
        m_Events.push_back(event);
    }
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

const CEventComponent::TEvenetHandlersMap& CEventComponent::Handlers() const
{
    return m_Handlers;
}

IEventPtr CEventComponent::Pop()
{
    if (m_Events.empty())
    {
        return nullptr;
    }
 
    IEventPtr event = nullptr;
    if (m_DispatchCount)
    {
        event = m_Events.front();
        m_Events.erase(m_Events.begin());
        m_DispatchCount--;
        return event;
    }
    else
    {
        event = m_Events.back();
        m_Events.pop_back();
    }
    return event;
}

bool CEventComponent::DispatchEvent(IEventPtr event)
{
    TEvenetHandlersMap::const_iterator it = m_Handlers.find(event->GetId());
    if (it == m_Handlers.end())
    {
        return true;
    }
    
    const TEventHandler& eventHandler = it->second;
    return eventHandler(event);
}
