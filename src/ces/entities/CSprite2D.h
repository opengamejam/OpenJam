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

namespace jam {

CLASS_PTR(CSprite2D)
CLASS_PTR(CRenderComponent)
CLASS_PTR(CAnimation2DComponent)
CLASS_PTR(CTransformationComponent)
CLASS_PTR(IRenderer)

class CSprite2D : public CEntityBase {
public:
    static CSprite2DPtr Create(const std::string& filename, IRendererPtr renderer);

    CSprite2D();
    virtual ~CSprite2D();

    CAnimation2DComponentPtr AnimationComponent() const;

private:
    CAnimation2DComponentPtr m_AnimationComponent;
};

}; // namespace jam

#endif /* defined(CSPRITE2D_H) */

