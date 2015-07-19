//
//  IAnimator.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IANIMATOR_H
#define IANIMATOR_H

namespace jam
{

class IAnimator
{
public:
    typedef std::function<void(void)> TCompletionHandler;
    
public:
    IAnimator() = default;
    virtual ~IAnimator() = default;
    
    virtual void GotoAnimation(unsigned int index) = 0;
    virtual void GotoAnimation(const std::string& name) = 0;
    
    virtual void GotoAndStop(float percent) = 0;
    virtual void GotoAndStop(unsigned long time) = 0;
    
    virtual void GotoAndPlay(float percent, unsigned int repeat = -1, TCompletionHandler handler = nullptr) = 0;
    virtual void GotoAndPlay(unsigned long time, unsigned int repeat = -1, TCompletionHandler handler = nullptr) = 0;
    
    virtual float GetCurrentProgress() const = 0;
    virtual unsigned long GetCurrentTime() const = 0;
    virtual unsigned long GetFullTime() const = 0;
    
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    
    virtual void Update(unsigned long dt) = 0;
};

}; // namespace jam

#endif /* IANIMATOR_H */
