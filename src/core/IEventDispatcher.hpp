//
//  IEventDispatcher.hpp
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IEVENTDISPATCHER_H
#define IEVENTDISPATCHER_H

#include "Global.h"
#include "IEvent.h"
#include <typeindex>

namespace jam
{
CLASS_PTR(IEvent)
    
class IEventDispatcher final
{
public:
    typedef std::function<bool(IEventPtr event)> TEventHandler;
    typedef std::vector<TEventHandler> TEventHandlersList;
    typedef std::unordered_map<typeid_t, TEventHandlersList> TEvenetHandlersMap;
    
public:
    IEventDispatcher() = default;
    ~IEventDispatcher() = default;
    
    template<typename T>
    void RegisterEventHandler(const TEventHandler& eventHandler)
    {
        typeid_t eventType = CTypeId<T>::Id();
        m_HandlersMap[eventType].push_back(eventHandler);
    }
    
    template <typename T>
    void UnregisterEventHandler(const TEventHandler& eventHandler)
    {
        typeid_t eventType = CTypeId<T>::Id();
        
        TEventHandlersList::const_iterator beg = m_HandlersMap[eventType].begin();
        TEventHandlersList::const_iterator end = m_HandlersMap[eventType].end();
        while (beg != end)
        {
            
            m_HandlersMap[eventType].erase(beg);
            ++beg;
        }
    }
    
    template <typename T>
    void UnregisterEventHandlers()
    {
        typeid_t eventType = CTypeId<T>::Id();
        
        m_HandlersMap[eventType].clear();
    }
    
    void DispatchEvent(IEventPtr event)
    {
        typeid_t eventType = event->GetId();
        
        TEventHandlersList handlers = m_HandlersMap[eventType];
        std::all_of(handlers.begin(), handlers.end(), [&event](TEventHandler handler)
        {
            if (handler)
            {
                return handler(event); // stop propagation
            }
            
            return false;
        });
    }
    
    void AddEvent(IEventPtr event)
    {
        m_Events.push_back(event);
    }
    
    void AddEvents(std::vector<IEventPtr> events)
    {
        std::copy(events.begin(), events.end(), std::back_inserter(m_Events));
    }
    
    void Update(unsigned long dt)
    {
        std::for_each(m_Events.begin(), m_Events.end(), [&](IEventPtr event)
        {
            DispatchEvent(event);
        });
        m_Events.clear();
    }
    
private:
    TEvenetHandlersMap m_HandlersMap;
    std::vector<IEventPtr> m_Events;
};

}; // namespace jam

#endif /* IEVENTDISPATCHER_H */
