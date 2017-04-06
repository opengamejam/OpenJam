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

namespace jam {

CLASS_PTR(CObject3D)
CLASS_PTR(CRenderComponent)
CLASS_PTR(CTransformationComponent)
CLASS_PTR(IRenderer)

class CObject3D : public CEntityBase {
public:
    // Load mesh from .obj 3D file
    static CObject3DPtr CreateObj(const std::string& filename, IRendererPtr renderer);

    CObject3D();
    virtual ~CObject3D();
};

}; // namespace jam

#endif /* defined(CObject3D_H) */

