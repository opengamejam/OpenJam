//
//  IUnique.c
//  TestApp
//
//  Created by Yevgeniy Logachev on 8/26/15.
//
//

#include "IUnique.h"

using namespace jam;

uint64_t Unique::s_NextId = 0;

Unique::Unique()
: m_UniqueId(NextId())
{

}

Unique::~Unique()
{

}

uint64_t Unique::UniqueId() const
{
    return m_UniqueId;
}

uint64_t Unique::NextId()
{
    return s_NextId++;
}