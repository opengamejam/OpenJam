//
//  CRenderComponent.cpp
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#include "CBatchComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CBatchComponent::CBatchComponent()
: IComponent(ComponentId<CBatchComponent>())
, m_BatchId(0)
, m_Transform()
, m_OldTransform()
, m_OffsetVertex(0)
, m_SizeVertex(0)
, m_OffsetIndex(0)
, m_SizeIndex(0)
{
    
}

CBatchComponent::~CBatchComponent()
{
    
}

uint64_t CBatchComponent::BatchId() const
{
    return m_BatchId;
}

void CBatchComponent::BatchId(uint64_t batchId)
{
    m_BatchId = batchId;
}

const CTransform3Df& CBatchComponent::Transform() const
{
    return m_Transform;
}

void CBatchComponent::Transform(const CTransform3Df& transform)
{
    m_OldTransform = m_Transform;
    m_Transform = transform;
}

const CTransform3Df& CBatchComponent::OldTransform() const
{
    return m_OldTransform;
}

uint64_t CBatchComponent::OffsetVertex() const
{
    return m_OffsetVertex;
}

void CBatchComponent::OffsetVertex(uint64_t offsetVertex)
{
    m_OffsetVertex = offsetVertex;
}

uint64_t CBatchComponent::SizeVertex() const
{
    return m_SizeVertex;
}

void CBatchComponent::SizeVertex(uint64_t sizeVertex)
{
    m_SizeVertex = sizeVertex;
}

uint64_t CBatchComponent::OffsetIndex() const
{
    return m_OffsetIndex;
}

void CBatchComponent::OffsetIndex(uint64_t offsetIndex)
{
    m_OffsetIndex = offsetIndex;
}

uint64_t CBatchComponent::SizeIndex() const
{
    return m_SizeIndex;
}

void CBatchComponent::SizeIndex(uint64_t sizeIndex)
{
    m_SizeIndex = sizeIndex;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************