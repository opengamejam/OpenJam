//
//  CSprite2D.h
//  Arkanoid
//
//  Created by yev on 5/15/15.
//
//

#ifndef CSPRITE2D_H
#define CSPRITE2D_H

#include "IEntity.h"
#include "CVector3D.h"

namespace jam
{
    
CLASS_PTR(CSprite2D);
CLASS_PTR(CRenderComponent);
CLASS_PTR(CAnimation2DComponent);
CLASS_PTR(CTransformationComponent);

class CSprite2D : public IEntity
{
public:
    static CSprite2DPtr Create(const std::string& name);
    
    CSprite2D();
    virtual ~CSprite2D();
    
    CRenderComponentPtr RenderComponent() const;
    CAnimation2DComponentPtr AnimationComponent() const;
    CTransformationComponentPtr TransformComponent() const;
    
    void Position(const CVector3Df& position);
    const CVector3Df& Position();
    
    void Rotation(const CVector3Df& rotation);
    const CVector3Df& Rotation();
    
    void Scale(const CVector3Df& scale);
    const CVector3Df& Scale();
    
    void AnchorPoint(const CVector3Df& anchorPoint);
    const CVector3Df& AnchorPoint();
    
private:
    CRenderComponentPtr m_RenderComponent;
    CAnimation2DComponentPtr m_AnimationComponent;
    CTransformationComponentPtr m_TransformationComponent;
};
    
}; // namespace jam

#endif /* defined(CSPRITE2D_H) */
