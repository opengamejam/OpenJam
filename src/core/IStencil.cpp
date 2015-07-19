//
//  IStencil.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/20/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "IStencil.h"

using namespace jam;

IStencil::IStencil(StencilFunc func, unsigned int ref, unsigned int mask,
                            StencilOp failOp, StencilOp zfailOp, StencilOp zpassOp)
    : m_Func(func)
    , m_Ref(ref)
    , m_Mask(mask)
    , m_FailOp(failOp)
    , m_ZFailOp(zfailOp)
    , m_ZPassOp(zpassOp)
{

}


IStencil::~IStencil()
{

}

void IStencil::Func(StencilFunc func)
{
    m_Func = func;
}

IStencil::StencilFunc IStencil::Func() const
{
    return m_Func;
}

void IStencil::Ref(unsigned int ref)
{
    m_Ref = ref;
}

unsigned int IStencil::Ref() const
{
    return m_Ref;
}

void IStencil::Mask(unsigned int mask)
{
    m_Mask = mask;
}

unsigned int IStencil::Mask() const
{
    return m_Mask;
}

void IStencil::FailOp(StencilOp failOp)
{
    m_FailOp = failOp;
}

IStencil::StencilOp IStencil::FailOp() const
{
    return m_FailOp;
}

void IStencil::ZFailOp(StencilOp zfailOp)
{
    m_ZFailOp = zfailOp;
}

IStencil::StencilOp IStencil::ZFailOp() const
{
    return m_ZFailOp;
}

void IStencil::ZPassOp(StencilOp zpassOp)
{
    m_ZPassOp = zpassOp;
}

IStencil::StencilOp IStencil::ZPassOp() const
{
    return m_ZPassOp;
}


