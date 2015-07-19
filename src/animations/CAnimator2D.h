//
//  CAnimator2D.h
//  OpenJam
//
//  Created by yev on 5/14/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CANIMATOR2D_H
#define CANIMATOR2D_H

#include "Global.h"
#include "IAnimator.h"
#include "ISprite.h"


namespace jam
{
CLASS_PTR(ISprite);
CLASS_PTR(CAnimator2D);
    
    
class CAnimator2D : public IAnimator
{
public:
    typedef std::function<void(const CFrameDef&)> TFrameUpdatedHandler;
    
public:
    CAnimator2D(ISpritePtr sprite);
    virtual ~CAnimator2D();
    
    virtual void GotoAnimation(unsigned int index);
    virtual void GotoAnimation(const std::string& name);
    
    virtual void GotoAndStop(float percent);
    virtual void GotoAndStop(unsigned long time);
    
    virtual void GotoAndPlay(float percent, unsigned int repeat = -1, TCompletionHandler handler = nullptr);
    virtual void GotoAndPlay(unsigned long time, unsigned int repeat = -1, TCompletionHandler handler = nullptr);
    
    virtual float GetCurrentProgress() const;
    virtual unsigned long GetCurrentTime() const;
    virtual unsigned long GetFullTime() const;
    
    virtual void Play();
    virtual void Stop();
    virtual void Pause();
    
    virtual void Update(unsigned long dt);
    
    void OnFrameUpdated(TFrameUpdatedHandler handler);
    
private:
    ISpritePtr m_Sprite;
    TFrameUpdatedHandler m_Handler;
    TCompletionHandler m_LoopHandler;
    
    unsigned int                        m_CurrentAnimIndex;
    unsigned int                        m_LastAnimIndex;
    unsigned int                        m_CurrentFrameIndex;
    unsigned int                        m_LastFrameIndex;
    unsigned long                       m_LastFrameTime;
    
    bool                                m_IsPlaying;    
    int                                 m_RepeatCounter;    // -1 - infinite
};

}; // namespace jam

#endif /* defined(CANIMATOR2D_H) */
