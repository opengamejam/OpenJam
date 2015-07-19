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
public:
    enum RenderApi
    {
        OGL1,
        OGL3,
        OGL4
    };
    
public:
	CRenderViewOSX(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi);
	virtual ~CRenderViewOSX();

	virtual void CreateView();
    virtual void Begin() const;
    virtual void End() const;
    virtual void UpdateEvents() const;
    
    virtual IRenderTargetPtr DefaultRenderTarget() const;

private:
#if defined(__OBJC__)
  	NSOpenGLView* m_GLView;
    NSOpenGLContext* m_GLContext;
#endif
    RenderApi m_RenderApi;
    IRenderTargetPtr m_DefaultRenderTarget;
};

}; // namespace jam

#endif	/* CRENDERVIEWOSX_H */

#endif /* OS_MAC */

