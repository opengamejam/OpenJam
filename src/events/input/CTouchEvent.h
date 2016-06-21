//
//  CTouchEvent.h
//  OpenJam
//
//  Created by yev on 5/14/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CTOUCHEVENT_H
#define CTOUCHEVENT_H

#include "IEvent.h"
#include "CMath.h"

namespace jam
{
CLASS_PTR(CTouchEvent)  // TODO:

class CTouchEvent : public IEvent
{
    JAM_OBJECT
public:
    enum ETouchEventType
    {
        Down = 0,
        Move,
        Up,
        Reset
    };
    
public:
    CTouchEvent(const glm::vec2& position, ETouchEventType type);
    virtual ~CTouchEvent();
    
    virtual const std::string& Id() const override;
    
    const glm::vec2& GetPosition() const;
    ETouchEventType GetType() const;
    
private:
    std::string m_Id;
    glm::vec2 m_Position;
    ETouchEventType m_Type;
};

}; // namespace jam

#endif /* defined(CTOUCHEVENT_H) */
