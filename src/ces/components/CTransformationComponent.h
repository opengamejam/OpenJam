//
//  CTransformationComponent.h
//  OpenJam
//
//  Created by yev on 2/6/15.
//
//

#ifndef CTRANSFORMATIONCOMPONENT_H
#define CTRANSFORMATIONCOMPONENT_H

#include "IComponent.h"
#include "CTransform.hpp"

namespace jam {
CLASS_PTR(CTransformationComponent)
    
/*
 * CTransformationComponent contains info about all applied entity transformations 
 * and its absolute transformation
 */
class CTransformationComponent final : public CComponentBase {
    JAM_OBJECT
public:
    /*
     * Several standard transformation types.
     * Local - model transformation
     * Parent - transformation of parent entity
     * Animation - transformation from animation system
     */
    enum TransformationTypes {
        Local = 0,
        Parent,
        Animation
    };

    /*
     * Transformation and it properties. if isAffectsOnChilds is set to 'true'
     * then current transform won't be applied to their children
     */
    struct STransformProps {
        CTransform3Df transform;
        bool isAffectsOnChilds;

        STransformProps(const CTransform3Df& _transform = CTransform3Df(),
                        bool _isAffectsOnChilds = false);
    };
    typedef std::unordered_map<int, STransformProps> TTransformsList;

public:
    CTransformationComponent();
    virtual ~CTransformationComponent();

    /*
     * Add or update transformation
     */
    void AddTransform(int key, const CTransform3Df& transform, bool isAffectsOnChilds = true);
    
    /*
     * Remove existing transformation
     */
    void RemoveTransform(int key);
    
    /*
     * Extract transformation by the key
     */
    const CTransform3Df& Transform(int key);
    
    /*
     * Check if a transformation exists by the key
     */
    bool HasTransform(int key) const;
    
    /*
     * Get full transformation list
     */
    const TTransformsList& Transformations() const;

    /*
     * Recalculate absolute and children transformations
     */
    void UpdateAbsoluteTransform();
    
    /*
     * Get absolute transformation
     */
    const CTransform3Df& AbsoluteTransformation();
    
    /*
     * Get absolute transformation without transformation, that hasn't been applied
     * to children ('isAffectsOnChilds' flag of such transformation is set to true)
     */
    const CTransform3Df& ChildrenTransformation();

private:
    TTransformsList m_Transformations;
    CTransform3Df m_AbsoluteTransformation;
    CTransform3Df m_ChildrenTransformation;
};

}; // namespace jam

#endif /* defined(CTRANSFORMATIONCOMPONENT_H) */
