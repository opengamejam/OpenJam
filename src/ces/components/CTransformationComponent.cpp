//
//  CTransformationComponent.cpp
//  Arkanoid
//
//  Created by yev on 2/6/15.
//
//

#include "CTransformationComponent.h"
#include "IEntity.h"

using namespace jam;

CTransformationComponent::CTransformationComponent()
: IComponent(ComponentId<CTransformationComponent>())
{
}

CTransformationComponent::~CTransformationComponent()
{

}

void CTransformationComponent::AddTransform(int key,
                                            const CTransform3Df& transform,
                                            bool isAffectsOnChilds)
{
    m_Transforms[key] = STransformProps(transform, isAffectsOnChilds);
}

void CTransformationComponent::RemoveTransform(int key)
{
    m_Transforms.erase(key);
}

const CTransform3Df& CTransformationComponent::Transform(int key)
{    
    return m_Transforms[key].transform;
}

bool CTransformationComponent::HasTransform(int key) const
{
    return (m_Transforms.find(key) != m_Transforms.end());
}

const CTransformationComponent::TTransformsList& CTransformationComponent::Transforms() const
{
    return m_Transforms;
}

void CTransformationComponent::ResultTransform(const CTransform3Df& transform)
{
    m_ResultTransform = transform;
}

const CTransform3Df& CTransformationComponent::ResultTransform()
{
    return m_ResultTransform;
}