//
//  CCamera2D.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCAMERA2D_H
#define CCAMERA2D_H

#include "ICamera.h"
#include "CMath.h"

namespace jam {
CLASS_PTR(CCamera2D)

class CCamera2D : public ICamera {
public:
    /*
     * Create new 2D camera with added transformation component
     */
    static CCamera2DPtr Create(float _near = -10000.0f, float _far = 10000.0f);

    /*
     * Default constructor. To create new camera use static method Create() instead
     */
    CCamera2D(float _near = -10000.0f, float _far = 10000.0f);
    
    /*
     * Destructor
     */
    virtual ~CCamera2D();

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
    float m_Near;
    float m_Far;
    glm::mat4x4 m_ProjectionMatrix;
    IFrameBufferPtr m_FrameBuffer;

    bool m_IsFlippedX;
    bool m_IsFlippedY;
};

}; // namespace jam

#endif /* CCAMERA2D_H */
