//
//  CRenderViewX11.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_LINUX)

#ifndef CRENDERVIEWX11_H
#define	CRENDERVIEWX11_H

#include "X11/Xlib.h"
#include "X11/Xutil.h"

#include "IRenderView.h"

namespace jam
{

class CRenderViewX11 : public IRenderView
{
public:
	CRenderViewX11(unsigned int width, unsigned int height,
                   unsigned int targetWidth, unsigned int targetHeight);
	virtual ~CRenderViewX11();
    
	virtual void        CreateView();
	virtual void        SwapBuffer() const;
    virtual void        UpdateEvents() const;

private:
	EGLNativeWindowType m_EglWindow;
	EGLDisplay			m_EglDisplay;
	EGLSurface			m_EglSurface;
	EGLContext			m_EglContext;
    Window              m_Window;
    Display*            m_Display;
};

}; // namespace jam

#endif /* CRENDERVIEWX11_H */

#endif /* OS_LINUX */



