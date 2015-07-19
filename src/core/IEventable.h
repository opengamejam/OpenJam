//
//  IEventable.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IEVENTABLE_H
#define IEVENTABLE_H

#include "Global.h"

namespace jam
{
CLASS_PTR(IEventDispatcher);

class IEventable
{
public:
    IEventable() = default;
    virtual ~IEventable() = default;
    
    IEventDispatcherPtr Dispatcher() const;
    static void RegisterDispatcher(IEventDispatcherPtr eventDispatcher);
    
private:
    static IEventDispatcherPtr s_EventDispatcher;
};
    
}; // namespace jam

#endif /* defined(IEVENTABLE_H) */
