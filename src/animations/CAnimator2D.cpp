//
//  CAnimator2D.cpp
//  OpenJam
//
//  Created by yev on 5/14/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CAnimator2D.h"

using namespace jam;

CAnimator2D::CAnimator2D(ISpritePtr sprite)
    : m_Sprite(sprite)
    , m_Handler(nullptr)
    , m_LoopHandler(nullptr)
    , m_CurrentAnimIndex(0)
    , m_LastAnimIndex(-1)
    , m_CurrentFrameIndex(0)
    , m_LastFrameIndex(-1)
    , m_LastFrameTime(0)
    , m_IsPlaying(true)
    , m_RepeatCounter(-1)
{

}

CAnimator2D::~CAnimator2D()
{
}

void CAnimator2D::GotoAnimation(unsigned int index)
{
    m_CurrentAnimIndex = index;
    m_CurrentFrameIndex = 0;
}

void CAnimator2D::GotoAnimation(const std::string& name)
{
    const ISprite::TAnimationsList& animations = m_Sprite->Animations();
    unsigned int idx = 0;
    std::all_of(animations.begin(), animations.end(), [&](const CAnimationDef& animation)
    {
        if (animation.name == name)
        {
            m_CurrentAnimIndex = idx;
            m_CurrentFrameIndex = 0;
            return false;
        }
        
        ++idx;
        
        return true;
    });
}

void CAnimator2D::GotoAndStop(float percent)
{
    if (percent >= 0.0f && percent <= 1.0f)
    {
        size_t framesCount = m_Sprite->Animations()[m_CurrentAnimIndex].frames.size();
        
        m_CurrentFrameIndex = (unsigned int)(percent * framesCount);
    }
    
    m_IsPlaying = false;
}

void CAnimator2D::GotoAndStop(unsigned long time)
{
    const ISprite::TAnimationsList& animations = m_Sprite->Animations();
    if (m_CurrentAnimIndex >= animations.size())
    {
        return;
    }
    const ISprite::TFramesList& frames = animations[m_CurrentAnimIndex].frames;
    
    unsigned int frameIndex = 0;
    unsigned long tmpTime = 0;
    
    std::all_of(frames.begin(), frames.end(), [&](CFrameDef frame)
    {
        tmpTime += frame.animDuration;
        if (tmpTime >= time)
        {
            return true;
        }
        
        frameIndex++;
        
        return false;
    });
    
    m_CurrentFrameIndex = frameIndex;
    
    m_IsPlaying = false;
}

void CAnimator2D::GotoAndPlay(float percent, unsigned int repeat, TCompletionHandler handler)
{
    GotoAndStop(percent);
    
    m_RepeatCounter = repeat;
    m_LoopHandler = handler;
    m_IsPlaying = true;
}

void CAnimator2D::GotoAndPlay(unsigned long time, unsigned int repeat, TCompletionHandler handler)
{
    GotoAndStop(time);
    
    m_RepeatCounter = repeat;
    m_LoopHandler = handler;
    m_IsPlaying = true;
}

float CAnimator2D::GetCurrentProgress() const
{
    size_t framesCount = m_Sprite->Animations()[m_CurrentAnimIndex].frames.size();
    
    return (float)m_CurrentFrameIndex / framesCount;
}

unsigned long CAnimator2D::GetCurrentTime() const
{
    const ISprite::TAnimationsList& animations = m_Sprite->Animations();
    if (m_CurrentAnimIndex >= animations.size())
    {
        return 0;
    }
    const ISprite::TFramesList& frames = animations[m_CurrentAnimIndex].frames;
    
    unsigned int frameIndex = 0;
    unsigned long currentTime = 0;
    
    std::all_of(frames.begin(), frames.end(), [&](CFrameDef frame)
    {
        currentTime += frame.animDuration;
        if (frameIndex == m_CurrentFrameIndex)
        {
            return true;
        }
        
        frameIndex++;
        
        return false;
    });
    
    return currentTime;
}

unsigned long CAnimator2D::GetFullTime() const
{
    const ISprite::TAnimationsList& animations = m_Sprite->Animations();
    if (m_CurrentAnimIndex >= animations.size())
    {
        return 0;
    }
    const ISprite::TFramesList& frames = animations[m_CurrentAnimIndex].frames;
    
    unsigned long fullTime = 0;
    
    std::for_each(frames.begin(), frames.end(), [&fullTime](const CFrameDef& frame)
    {
        fullTime += frame.animDuration;
    });
    
    return fullTime;
}

void CAnimator2D::Play()
{
    m_IsPlaying = true;
}

void CAnimator2D::Stop()
{
    m_CurrentFrameIndex = 0;
    m_IsPlaying = false;
}

void CAnimator2D::Pause()
{
    m_IsPlaying = false;
}

void CAnimator2D::Update(unsigned long dt)
{
    if (!m_IsPlaying || !m_Sprite)
    {
        return;
    }
    
    const ISprite::TAnimationsList& animations = m_Sprite->Animations();
    if (m_CurrentAnimIndex >= animations.size())
    {
        return;
    }
    const ISprite::TFramesList& frames = animations[m_CurrentAnimIndex].frames;
    if (m_CurrentFrameIndex >= frames.size())
    {
        return;
    }
    
    m_LastFrameTime += dt;
    const CFrameDef& frame = frames[m_CurrentFrameIndex];
    if (m_LastFrameTime > frame.animDuration)
    {
        m_LastFrameTime = 0;
        
        if (m_RepeatCounter > 0 || m_RepeatCounter == -1)
        {
            ++m_CurrentFrameIndex;
        }
        if (m_CurrentFrameIndex >= frames.size())
        {
            if (m_RepeatCounter > 0)
            {
                m_RepeatCounter = std::max(0, --m_RepeatCounter);
            }
            m_CurrentFrameIndex = 0;
            
            if (m_RepeatCounter == 0 && m_LoopHandler)
            {
                m_LoopHandler();
            }
        }
        
        if (m_LastFrameIndex != m_CurrentFrameIndex ||
            m_LastAnimIndex != m_CurrentAnimIndex)
        {
            m_LastAnimIndex = m_CurrentAnimIndex;
            m_LastFrameIndex = m_CurrentFrameIndex;
            
            if (m_Handler)
            {
                m_Handler(frame);
            }
        }
    }
}

void CAnimator2D::OnFrameUpdated(TFrameUpdatedHandler handler)
{
    m_Handler = handler;
}