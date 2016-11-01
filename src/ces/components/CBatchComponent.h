//
//  CBatchComponent.h
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#ifndef CBATCHCOMPONENT_H
#define CBATCHCOMPONENT_H

#include "Global.h"
#include "IComponent.h"
#include "CTransform.hpp"

namespace jam {
CLASS_PTR(CBatchComponent)

class CBatchComponent final : public CComponentBase {
    JAM_OBJECT

    friend class CBatchingSystem;

public:
    CBatchComponent();
    virtual ~CBatchComponent();

    uint64_t BatchId() const;
    uint64_t OffsetVertex() const;
    uint64_t SizeVertex() const;
    uint64_t OffsetIndex() const;
    uint64_t SizeIndex() const;
    const CTransform3Df& OldTransform() const;
    const CTransform3Df& Transform() const;

private:
    void BatchId(uint64_t batchId);
    void OffsetVertex(uint64_t offsetVertex);
    void SizeVertex(uint64_t sizeVertex);
    void OffsetIndex(uint64_t offsetIndex);
    void SizeIndex(uint64_t sizeIndex);
    void Transform(const CTransform3Df& transform);

private:
    uint64_t m_BatchId;
    CTransform3Df m_Transform;
    CTransform3Df m_OldTransform;
    uint64_t m_OffsetVertex;
    uint64_t m_SizeVertex;
    uint64_t m_OffsetIndex;
    uint64_t m_SizeIndex;
};

}; // namespace jam

#endif /* defined(CBATCHCOMPONENT_H) */

