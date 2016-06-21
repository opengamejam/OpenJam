//
//  CEventComponent.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/14/16.
//
//

#ifndef CEVENTCOMPONENT_H
#define CEVENTCOMPONENT_H

#include "Global.h"
#include "IComponent.h"
#include "CSlot.hpp"

namespace jam
{
CLASS_PTR(IEvent);
CLASS_PTR(CEventComponent);

class CEventComponent final : public IComponent
{
    JAM_OBJECT
    
    friend class CEventSystem;
public:
    typedef CSlot<bool, IEventPtr> TEventHandler;
    typedef std::unordered_map<uint64_t, TEventHandler> TEvenetHandlersMap;
    typedef std::list<IEventPtr> TEventsList;
    
public:
    CEventComponent();
    ~CEventComponent();
    
    void RegisterHandler(typeid_t eventId, const TEventHandler& eventHandler);
    void UnregisterHandler(typeid_t eventId, const TEventHandler& eventHandler);
    
    void AddEvent(IEventPtr event, bool fire = false);
    
private:
    const TEvenetHandlersMap& Handlers() const;
    IEventPtr Pop();
    uint64_t DispatchCount() const;
    bool DispatchEvent(IEventPtr event);
    
private:
    TEvenetHandlersMap m_Handlers;
    TEventsList m_Events;
    uint64_t m_DispatchCount;
};
    
}; // namespace jam

#endif /* CEVENTCOMPONENT_H */
