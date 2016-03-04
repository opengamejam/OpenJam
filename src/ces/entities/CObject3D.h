//
//  CObject3D.h
//  Arkanoid
//
//  Created by yev on 5/15/15.
//
//

#ifndef CObject3D_H
#define CObject3D_H

#include "IEntity.h"

namespace jam
{
    
CLASS_PTR(CObject3D);
CLASS_PTR(CRenderComponent);
CLASS_PTR(CTransformationComponent);

class CObject3D : public IEntity
{
public:
    // Load mesh from .obj 3D file
    static CObject3DPtr CreateObj(const std::string& filename, 
								  uint32_t cameraId = std::numeric_limits<uint32_t>::max());
    
    CObject3D();
    virtual ~CObject3D();
    
    CRenderComponentPtr RenderComponent() const;
    CTransformationComponentPtr TransformComponent() const;
    
    void Position(const glm::vec3& position);
    const glm::vec3& Position();
    
    void Rotation(const glm::vec3& rotation);
    const glm::vec3& Rotation();
    
    void Scale(const glm::vec3& scale);
    const glm::vec3& Scale();
    
    void AnchorPoint(const glm::vec3& anchorPoint);
    const glm::vec3& AnchorPoint();
    
    void Update(unsigned long dt);

private:
    CRenderComponentPtr m_RenderComponent;
    CTransformationComponentPtr m_TransformationComponent;
};
    
}; // namespace jam

#endif /* defined(CObject3D_H) */
