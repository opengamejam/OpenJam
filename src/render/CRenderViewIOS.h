//
//  CRenderViewIOS.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_IPHONE)

#ifndef CRENDERVIEWIOS_H
#define	CRENDERVIEWIOS_H

#include "IRenderView.h"

namespace jam
{
class CRenderViewIOS : public IRenderView
{
public:
    enum RenderApi
    {
        OGLES1_1,
        OGLES2_0,
        OGLES3_0
    };
    
public:
	CRenderViewIOS(void* glkView, RenderApi renderApi);
	virtual ~CRenderViewIOS();

	virtual void CreateView() override;
    virtual void Begin() const override;
    virtual void End() const override;
    virtual void UpdateEvents() const override;
    
    virtual IRendererPtr Renderer() const override;
    virtual IRenderTargetPtr DefaultRenderTarget() const override;

private:
#if defined(__OBJC__)
  	UIView* m_GLKView;
    EAGLContext* m_GLContext;
#endif
    RenderApi m_RenderApi;
    unsigned int m_ColorBuffer;
    IRendererPtr m_Renderer;
    IRenderTargetPtr m_DefaultRenderTarget;
};

}; // namespace jam

#endif	/* CRENDERVIEWIOS_H */

#endif /* OS_IPHONE */

