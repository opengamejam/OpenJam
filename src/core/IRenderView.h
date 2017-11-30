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

class IRenderView : public std::enable_shared_from_this<IRenderView> {
    JAM_OBJECT_BASE
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
    
    template<class T, class = typename std::enable_if<std::is_base_of<IRenderInstance, T>::value, T>::type>
    std::shared_ptr<T> GetRenderInstance() const
    {
        IRenderInstancePtr instance = RenderInstance();
        if (instance->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(instance);
        }
        
        assert(false && "Trying to retrieve incorrect type of render instance");
        return nullptr;
    }
    
    virtual IRendererPtr Renderer() const = 0;
    
    template<class T, class = typename std::enable_if<std::is_base_of<IRenderer, T>::value, T>::type>
    std::shared_ptr<T> GetRenderer() const
    {
        IRendererPtr renderer = Renderer();
        if (renderer->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(renderer);
        }
        
        assert(false && "Trying to retrieve incorrect type of renderer");
        return nullptr;
    }
    
    virtual IFrameBufferPtr DefaultFrameBuffer() const = 0;
    
    template<class T>
    std::shared_ptr<T> GetDefaultFrameBuffer() const
    {
        IFrameBufferPtr frameBuffer = DefaultFrameBuffer();
        if (frameBuffer->GetId() == CTypeId<T>::Id()) {
            return std::static_pointer_cast<T>(frameBuffer);
        }
        
        assert(false && "Trying to retrieve incorrect type of default frame buffer");
        return nullptr;
    }
    
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

