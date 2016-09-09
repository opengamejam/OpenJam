//
//  CTouchComponent.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/31/16.
//
//

#include "CTouchComponent.h"

using namespace jam;

CTouchComponent::CTouchComponent()
{

}

CTouchComponent::~CTouchComponent()
{

}

void CTouchComponent::SetOnBeginTouch(const TTouchFunc& touchFunc)
{
    m_BeginTouchFunc = touchFunc;
}

void CTouchComponent::SetOnMoveTouch(const TTouchFunc& touchFunc)
{
    m_MoveTouchFunc = touchFunc;
}

void CTouchComponent::SetOnEndTouch(const TTouchFunc& touchFunc)
{
    m_EndTouchFunc = touchFunc;
}

void CTouchComponent::OnBeginTouch(const glm::vec2& touch)
{
    if (m_BeginTouchFunc)
    {
        m_BeginTouchFunc(touch);
    }
}

void CTouchComponent::OnMoveTouch(const glm::vec2& touch)
{
    if (m_MoveTouchFunc)
    {
        m_MoveTouchFunc(touch);
    }
}

void CTouchComponent::OnEndTouch(const glm::vec2& touch)
{
    if (m_EndTouchFunc)
    {
        m_EndTouchFunc(touch);
    }
}
