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
    static CSprite2DPtr Create(const std::string& filename,
        IRendererPtr renderer,
        uint32_t cameraId = std::numeric_limits<uint32_t>::max());

    CSprite2D();
    virtual ~CSprite2D();

    CRenderComponentPtr RenderComponent() const;
    CAnimation2DComponentPtr AnimationComponent() const;
    CTransformationComponentPtr TransformComponent() const;

    void Position(const glm::vec3& position);
    const glm::vec3& Position();

    void Rotation(const glm::vec3& rotation);
    const glm::vec3& Rotation();

    void Scale(const glm::vec3& scale);
    const glm::vec3& Scale();

    void AnchorPoint(const glm::vec3& anchorPoint);
    const glm::vec3& AnchorPoint();

private:
    CRenderComponentPtr m_RenderComponent;
    CAnimation2DComponentPtr m_AnimationComponent;
    CTransformationComponentPtr m_TransformationComponent;
};

}; // namespace jam

#endif /* defined(CSPRITE2D_H) */

