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
#include "CVector.hpp"

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
    
    void Position(const CVector3Df& position);
    const CVector3Df& Position();
    
    void Rotation(const CVector3Df& rotation);
    const CVector3Df& Rotation();
    
    void Scale(const CVector3Df& scale);
    const CVector3Df& Scale();
    
    void AnchorPoint(const CVector3Df& anchorPoint);
    const CVector3Df& AnchorPoint();
    
    void Update(unsigned long dt);

private:
    CRenderComponentPtr m_RenderComponent;
    CTransformationComponentPtr m_TransformationComponent;
};
    
}; // namespace jam

#endif /* defined(CObject3D_H) */
