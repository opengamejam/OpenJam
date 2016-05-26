//
//  CRenderComponent.cpp
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#include "CUpdateComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CUpdateComponent::CUpdateComponent(const TUpdateFunc& updateFunc)
: m_UpdateFunc(updateFunc)
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

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************