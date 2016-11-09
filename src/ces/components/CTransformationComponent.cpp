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

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTransformationComponent::CTransformationComponent()
{
}

CTransformationComponent::~CTransformationComponent()
{
}

void CTransformationComponent::AddTransform(int key,
    const CTransform3Df& transform,
    bool isAffectsOnChilds)
{
    m_Transformations[key] = STransformProps(transform, isAffectsOnChilds);
}

void CTransformationComponent::RemoveTransform(int key)
{
    m_Transformations.erase(key);
}

const CTransform3Df& CTransformationComponent::Transform(int key)
{
    return m_Transformations[key].transform;
}

bool CTransformationComponent::HasTransform(int key) const
{
    return (m_Transformations.find(key) != m_Transformations.end());
}

const CTransformationComponent::TTransformsList& CTransformationComponent::Transformations() const
{
    return m_Transformations;
}

void CTransformationComponent::UpdateAbsoluteTransform()
{
    m_AbsoluteTransformation = CTransform3Df();
    m_ChildrenTransformation = CTransform3Df();
    CTransform3Df parentTransform = Transform(CTransformationComponent::Parent);
    
    const CTransformationComponent::TTransformsList& transforms = Transformations();
    std::for_each(transforms.begin(), transforms.end(),
                  [&](const std::pair<int, CTransformationComponent::STransformProps>& element) {
        
        int type = element.first;
        const CTransformationComponent::STransformProps& transformProps = element.second;
        if (type != CTransformationComponent::Parent) {
            if (transformProps.isAffectsOnChilds)
            {
                m_ChildrenTransformation += transformProps.transform;
            }
            m_AbsoluteTransformation += transformProps.transform;
        }
    });
    m_AbsoluteTransformation.Parent(parentTransform());
    m_ChildrenTransformation.Parent(parentTransform());
}

const CTransform3Df& CTransformationComponent::AbsoluteTransformation()
{
    return m_AbsoluteTransformation;
}

const CTransform3Df& CTransformationComponent::ChildrenTransformation()
{
    return m_ChildrenTransformation;
}


CTransformationComponent::STransformProps::STransformProps(const CTransform3Df& _transform,
                                                           bool _isAffectsOnChilds)
    : transform(_transform)
    , isAffectsOnChilds(_isAffectsOnChilds)
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
