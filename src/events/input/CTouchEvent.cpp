//
//  CTouchEvent.cpp
//  OpenJam
//
//  Created by yev on 5/14/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CTouchEvent.h"

using namespace jam;

CTouchEvent::CTouchEvent(const glm::vec2& position, ETouchEventType type)
    : m_Id("CTouchEvent")
    , m_Position(position)
    , m_Type(type)
{

}

CTouchEvent::~CTouchEvent()
{

}

const std::string& CTouchEvent::Id() const
{
    return m_Id;
}

const glm::vec2& CTouchEvent::GetPosition() const
{
    return m_Position;
}

CTouchEvent::ETouchEventType CTouchEvent::GetType() const
{
    return m_Type;
}