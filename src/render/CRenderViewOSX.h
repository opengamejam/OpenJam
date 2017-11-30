//
//  CRenderViewOSX.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_MAC)

#ifndef CRENDERVIEWOSX_H
#define	CRENDERVIEWOSX_H

#include "IRenderView.h"

namespace jam
{
class CRenderViewOSX : public IRenderView
{
    JAM_OBJECT
public:
    enum RenderApi
    {
        OGLLegacy,
        OGL3_2,
        OGL4_1
    };
    
public:
	CRenderViewOSX(uint32_t width, uint32_t height, void* glkView, RenderApi renderApi);
	virtual ~CRenderViewOSX();

	virtual void CreateView() override;
    virtual void Begin() const override;
    virtual void End() const override;
    virtual void UpdateEvents() const override;
    
    virtual IRendererPtr Renderer() const override;
    virtual IFrameBufferPtr DefaultFrameBuffer() const override;

private:
#if defined(__OBJC__)
  	NSOpenGLView* m_GLView;
    NSOpenGLContext* m_GLContext;
#endif
    RenderApi m_RenderApi;
    IRendererPtr m_Renderer;
    IFrameBufferPtr m_DefaultFrameBuffer;
};

}; // namespace jam

#endif	/* CRENDERVIEWOSX_H */

#endif /* OS_MAC */

