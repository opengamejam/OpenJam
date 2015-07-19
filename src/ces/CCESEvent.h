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
    
    virtual const std::string& Id() const {return m_Id;}
    
    IEntityWeak Entity() const;
    ActionType Action() const;
    
private:
    IEntityWeak m_Entity;
    ActionType m_Action;
    std::string m_Id;
};

}; // namespace jam

#endif /* CCESEvent_H */
