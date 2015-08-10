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
        OGLES1,
        OGLES2,
        OGLES3
    };
    
public:
	CRenderViewIOS(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi);
	virtual ~CRenderViewIOS();

	virtual void        CreateView();
    virtual void        Begin() const;
    virtual void        End() const;
    virtual void        UpdateEvents() const;
    
    virtual IRenderTargetPtr DefaultRenderTarget() const;

private:
#if defined(__OBJC__)
  	UIView*             m_GLKView;
    EAGLContext*        m_GLContext;
#endif
    RenderApi m_RenderApi;
    unsigned int m_ColorBuffer;
    IRenderTargetPtr m_DefaultRenderTarget;
};

}; // namespace jam

#endif	/* CRENDERVIEWIOS_H */

#endif /* OS_IPHONE */

