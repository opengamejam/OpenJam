//
//  CRenderComponent.cpp
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#include "CUpdateComponent.h"

using namespace jam;

CUpdateComponent::CUpdateComponent(const TUpdateFunc& updateFunc)
: IComponent(ComponentId<CUpdateComponent>())
, m_UpdateFunc(updateFunc)
{

}

CUpdateComponent::~CUpdateComponent()
{

}

void CUpdateComponent::SetUpdateFunc(const TUpdateFunc& updateFunc)
{
    m_UpdateFunc = updateFunc;
}

bool CUpdateComponent::Update(unsigned long dt)
{
    if (m_UpdateFunc)
    {
        m_UpdateFunc(dt);
        
        return true;
    }
    
    return false;
}