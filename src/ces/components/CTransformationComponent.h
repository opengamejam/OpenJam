//
//  CTransformationComponent.h
//  Arkanoid
//
//  Created by yev on 2/6/15.
//
//

#ifndef __Arkanoid__CTransformationComponent__
#define __Arkanoid__CTransformationComponent__

#include "IComponent.h"
#include "CTransform.hpp"

namespace jam
{
CLASS_PTR(CTransformationComponent)

class CTransformationComponent final : public CComponentBase
{
    JAM_OBJECT    
public:
    enum TransformFlags
    {
        World = 0x00,
        Local = 0x01,
        Animation = 0x02
    };
    
    struct STransformProps
    {
        CTransform3Df transform;
        bool isAffectsOnChilds;
        
        STransformProps(const CTransform3Df& _transform = CTransform3Df(),
                        bool _isAffectsOnChilds = false)
        : transform(_transform)
        , isAffectsOnChilds(_isAffectsOnChilds)
        {
        }
    };
    
    typedef std::unordered_map<int, STransformProps> TTransformsList;
    
public:
    CTransformationComponent();
    virtual ~CTransformationComponent();
    
    void AddTransform(int key, const CTransform3Df& transform, bool isAffectsOnChilds = true);
    void RemoveTransform(int key);
    const CTransform3Df& Transform(int key);
    bool HasTransform(int key) const;
    const TTransformsList& Transforms() const;
    
    void ResultTransform(const CTransform3Df& transform);
    const CTransform3Df& ResultTransform();
    
private:
    TTransformsList m_Transforms;    
    CTransform3Df m_ResultTransform;
};
    
}; // namespace jam

#endif /* defined(__Arkanoid__CTransformationComponent__) */
