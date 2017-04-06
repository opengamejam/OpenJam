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
     * Returns Projection View matrix. Attribute 'frameBufferIndex' choose projection matrix for according frame buffer
     * if there are several atached frame buffers
     */
    virtual glm::mat4x4 ProjectionMatrix(uint64_t frameBufferIndex = kDefaultFrameBuffer) override;
    
    /*
     * Return attached framebuffer. Attribute 'frameBufferIndex' choose required frame buffer
     * if there are several atached frame buffers
     */
    virtual IFrameBufferPtr FrameBuffer(uint64_t frameBufferIndex = kDefaultFrameBuffer) const override;
    
    /*
     * Assign new framebuffer. Projection matrix will be recalculated to current framebuffer width and height.
     * Attribute 'frameBufferIndex' choose required frame buffer if there are several atached frame buffers
     * Pass nullptr to remove framebuffer at position 'frameBufferIndex'
     */
    virtual void FrameBuffer(IFrameBufferPtr frameBuffer, uint64_t frameBufferIndex = kDefaultFrameBuffer) override;
    
    /*
     * Returns count of attached frame buffers
     */
    virtual uint64_t FrameBufferCount() const override;

    /*
     * Flip camera vertically
     */
    virtual void FlipY() override;
    
    /*
     * Flip camera horizontally
     */
    virtual void FlipX() override;
    
private:
    glm::mat4 Ortho(uint32_t w, uint32_t h, bool flipX, bool flipY);

private:
    float m_Near;
    float m_Far;
    std::vector<glm::mat4x4> m_ProjectionMatrix;
    std::vector<IFrameBufferPtr> m_FrameBuffer;

    bool m_IsFlippedX;
    bool m_IsFlippedY;
};

}; // namespace jam

#endif /* CCAMERA2D_H */
