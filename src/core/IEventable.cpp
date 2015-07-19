//
//  IEventable.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#include "IEventable.h"
#include "IEventDispatcher.hpp"

using namespace jam;

IEventDispatcherPtr IEventable::s_EventDispatcher = nullptr;

IEventDispatcherPtr IEventable::Dispatcher() const
{
    return s_EventDispatcher;
}

void IEventable::RegisterDispatcher(IEventDispatcherPtr eventDispatcher)
{
    s_EventDispatcher = eventDispatcher;
}