//
//  IMesh.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IRENDERVIEW_H
#define IRENDERVIEW_H

#include "Global.h"
#include "CSignal.hpp"
#include "IRenderInstance.h"
#include "IRenderer.h"
#include "IFrameBuffer.h"

namespace jam {
    
CLASS_PTR(IRenderView)
CLASS_PTR(CTouchEvent)

class IRenderView : public IObject<IRenderView> {
public:
    IRenderView(uint32_t width, uint32_t height, float scaleFactor = 1.0f)
        : m_Width(width)
        , m_Height(height)
        , m_ScaleFactor(scaleFactor)
    {
    }
    virtual ~IRenderView() = default;

    virtual void CreateView() = 0;
    virtual void Begin() const = 0;
    virtual void End() const = 0;
    virtual void UpdateEvents() const = 0;

    virtual IRenderInstancePtr RenderInstance() const = 0;
    virtual IRendererPtr Renderer() const = 0;
    virtual IFrameBufferPtr DefaultFrameBuffer() const = 0;
    
    INL uint32_t Width() const { return m_Width; }
    INL uint32_t Height() const { return m_Height; }

    INL uint32_t RealWidth() const { return Width() * ScaleFactor(); }
    INL uint32_t RealHeight() const { return Height() * ScaleFactor(); }
    
    INL float ScaleFactor() const { return m_ScaleFactor; }

    CSignal<void, CTouchEventPtr> OnTouchSignal;

private:
    uint32_t m_Width;
    uint32_t m_Height;
    float m_ScaleFactor;
};

}; // namespace jam

#endif /* IRENDERVIEW_H */

