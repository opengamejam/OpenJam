//
//  CRenderViewWindows.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_WINDOWS)

#ifndef CRENDERVIEWWINDOWS_H
#define	CRENDERVIEWWINDOWS_H

#include "IRenderView.h"

namespace jam
{

class CRenderViewWindows : public IRenderView
{
public:
	CRenderViewWindows(unsigned int width, unsigned int height, HINSTANCE hInstance);
	virtual ~CRenderViewWindows();

	virtual void CreateView();
	virtual void SwapBuffer() const;
    virtual void UpdateEvents() const;
    
private:
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	EGLNativeWindowType m_eglWindow;
	EGLDisplay			m_eglDisplay;
	EGLSurface			m_eglSurface;
	EGLContext			m_eglContext;
	HWND				m_Window;
	HDC					m_DeviceContext;
	HINSTANCE           m_Instance;
};

}; // namespace jam

#endif	/* CRENDERVIEWWINDOWS_H */

#endif /* OS_WINDOWS */

