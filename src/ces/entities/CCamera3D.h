//
//  CCamera3D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCAMERA3D_H
#define CCAMERA3D_H

#include "ICamera.h"
#include "CMath.h"

namespace jam {
CLASS_PTR(CCamera3D)

class CCamera3D : public ICamera {
public:
    /*
     * Create new 3D camera with added transformation component
     */
    static CCamera3DPtr Create(float _fov, float _near = 0.3f, float _far = 1000.0f);

    /*
     * Default constructor. To create new camera use static method Create() instead
     */
    CCamera3D(float _fov, float _near = 0.3f, float _far = 1000.0f);
    
    /*
     * Destructor
     */
    virtual ~CCamera3D();

    /*
     * Returns Projection View matrix
     */
    virtual glm::mat4x4 ProjectionMatrix() override;
    
    /*
     * Return attached framebuffer
     */
    virtual IFrameBufferPtr FrameBuffer() const override;
    
    /*
     * Assign new framebuffer. Projection matrix will be recalculated to current framebuffer width and height
     */
    virtual void FrameBuffer(IFrameBufferPtr frameBuffer) override;
    
    /*
     * Flip camera vertically
     */
    virtual void FlipY() override;
    
    /*
     * Flip camera horizontally
     */
    virtual void FlipX() override;

private:
    float m_FOV;
    float m_Near;
    float m_Far;
    glm::mat4x4 m_ProjectionMatrix;
    IFrameBufferPtr m_FrameBuffer;
};

}; // namespace jam

#endif /* CCAMERA3D_H */
