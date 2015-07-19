//
//  IStencil.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/20/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef ISTENCIL_H
#define ISTENCIL_H

#include "Global.h"
#include "CColor.h"

namespace jam
{

class IStencil
{
public:
    enum StencilFunc
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };
    
    enum StencilOp
    {
        Keep = 0,
        Replace,
        Incr,
        Decr,
        Invert,
        IncrWrap,
        DecrWrap
    };
    
public:
    IStencil(StencilFunc func = Never, unsigned int ref = 0x00, unsigned int mask = 0x00,
             StencilOp failOp = Keep, StencilOp zfailOp = Keep, StencilOp zpassOp = Keep);
    virtual ~IStencil();
    
    virtual void                    Func(StencilFunc func);
    virtual StencilFunc             Func() const;
    virtual void                    Ref(unsigned int ref);
    virtual unsigned int            Ref() const;
    virtual void                    Mask(unsigned int mask);
    virtual unsigned int            Mask() const;
    virtual void                    FailOp(StencilOp failOp);
    virtual StencilOp               FailOp() const;
    virtual void                    ZFailOp(StencilOp zfailOp);
    virtual StencilOp               ZFailOp() const;
    virtual void                    ZPassOp(StencilOp zpassOp);
    virtual StencilOp               ZPassOp() const;

    
private:
    StencilFunc m_Func;
    unsigned int m_Ref;
    unsigned int m_Mask;
    
    StencilOp m_FailOp;
    StencilOp m_ZFailOp;
    StencilOp m_ZPassOp;
};
    
CLASS_PTR(IStencil);
    
}; // namespace jam

#endif // ISTENCIL_H
