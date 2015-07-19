//
//  IEvent.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef IEVENT_H
#define IEVENT_H

#include "Global.h"

namespace jam
{

class IEvent
{
public:
    IEvent() = default;
    virtual ~IEvent() = default;
    
    virtual const std::string& Id() const = 0;
};

}; // namespace jam

#endif /* IEVENT_H */
