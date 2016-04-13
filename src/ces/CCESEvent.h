//
//  CCESEvent.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CCESEvent_H
#define CCESEvent_H

#include "IEvent.h"

namespace jam
{
    
CLASS_WEAK(IEntity);
CLASS_WEAK(IComponent);
CLASS_PTR(CCESEvent);

class CCESEvent : public IEvent
{
public:
    enum ActionType
    {
        Added = 0,
        Removed,
        Changed
    };
    
public:
    CCESEvent(IEntityWeak entity, ActionType action);
    virtual ~CCESEvent();
    
    virtual const std::string& Id() const override;
    
    IEntityWeak Entity() const;
    ActionType Action() const;
    
    IComponentWeak Component() const;
    void Component(IComponentWeak component);
    
private:
    IEntityWeak m_Entity;
    IComponentWeak m_Component;
    ActionType m_Action;
    std::string m_Id;
};

}; // namespace jam

#endif /* CCESEvent_H */
