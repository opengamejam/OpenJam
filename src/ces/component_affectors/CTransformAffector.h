//
//  CTransformAffector.h
//  TestApp
//
//  Created by yev on 8/7/15.
//
//

#ifndef CTRANSFORMAFFECTOR_H
#define CTRANSFORMAFFECTOR_H

#include "Global.h"
#include "CVector3D.h"

namespace jam
{
CLASS_PTR(IEntity);
CLASS_PTR(CTransformationComponent);

class CTransformAffector
{
public:
    CTransformAffector() = default;
    ~CTransformAffector() = default;
    
    static void Translating(IEntityPtr entity, const CVector3Df& translation);
    static void Rotating(IEntityPtr entity, const CVector3Df& rotation);
    static void Scaling(IEntityPtr entity, const CVector3Df& scaling);
    
    static void Position(IEntityPtr entity, const CVector3Df& position);
    static void Rotation(IEntityPtr entity, const CVector3Df& rotation);
    static void Scale(IEntityPtr entity, const CVector3Df& scaling);
    
    static CVector3Df Position(IEntityPtr entity);
    static CVector3Df Rotation(IEntityPtr entity);
    static CVector3Df Scale(IEntityPtr entity);
    
    static void Translating(CTransformationComponentPtr component, const CVector3Df& translation);
    static void Rotating(CTransformationComponentPtr component, const CVector3Df& rotation);
    static void Scaling(CTransformationComponentPtr component, const CVector3Df& scaling);
    
    static void Position(CTransformationComponentPtr component, const CVector3Df& position);
    static void Rotation(CTransformationComponentPtr component, const CVector3Df& rotation);
    static void Scale(CTransformationComponentPtr component, const CVector3Df& scaling);
};
    
}; // namespace jam

#endif /* defined(CTRANSFORMAFFECTOR_H) */
