//
//  IUnique.c
//  TestApp
//
//  Created by Yevgeniy Logachev on 8/26/15.
//
//

#include "IUnique.h"

using namespace jam;

uint64_t IUnique::s_NextId = 0;

IUnique::IUnique()
: m_UniqueId(NextId())
{

}

IUnique::~IUnique()
{

}

uint64_t IUnique::UniqueId() const
{
    return m_UniqueId;
}

uint64_t IUnique::NextId()
{
    return s_NextId++;
}