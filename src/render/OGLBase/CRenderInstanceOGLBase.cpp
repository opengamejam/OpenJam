//
//  CRenderInstanceOGLBase.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 11/9/17.
//  Copyright © 2017 Yevgeniy Logachev. All rights reserved.
//

#include "CRenderInstanceOGLBase.h"
#import <OpenGLES/EAGL.h>
#import "GLKit/GLKit.h"

using namespace jam;

CRenderInstanceOGLBase::CRenderInstanceOGLBase(UIView* glkView,
                                               EAGLContext* glContext)
: m_GLKView(glkView)
, m_GLContext(glContext)
{
    
}

#if defined(EGLAPI)
CRenderInstanceOGLBase::CRenderInstanceOGLBase(EGLNativeWindowType eglWindow,
                                               EGLDisplay eglDisplay,
                                               EGLSurface eglSurface,
                                               EGLContext eglContext)
: m_EglWindow(eglWindow)
, m_EglDisplay(eglDisplay)
, m_EglSurface(eglSurface)
, m_EglContext(eglContext)
{
    
}
#endif

CRenderInstanceOGLBase::~CRenderInstanceOGLBase()
{
    
}

void CRenderInstanceOGLBase::Initialize()
{
    if (IsInitialized()) {
        JAM_LOG("CRenderInstanceOGLBase - trying to initialize twice")
        return;
    }
    
    m_IsInitialized = true;
}

void CRenderInstanceOGLBase::Shutdown()
{
    if (!IsInitialized()) {
        JAM_LOG("CRenderInstanceOGLBase - trying to shutdown not initialized object")
        return;
    }
    
    m_IsInitialized = false;
}

bool CRenderInstanceOGLBase::IsInitialized() const
{
    return m_IsInitialized;
}

#if defined(__OBJC__)
UIView* CRenderInstanceOGLBase::View() const
{
    return m_GLKView;
}

EAGLContext* CRenderInstanceOGLBase::Context() const
{
    return m_GLContext;
}
#endif

#if defined(EGLAPI)
const EGLNativeWindowType& CRenderInstanceOGLBase::EglWindow() const
{
    return m_EglWindow;
}

const EGLDisplay& CRenderInstanceOGLBase::EglDisplay() const
{
    return m_EglDisplay;
}

const EGLSurface& CRenderInstanceOGLBase::EglSurface() const
{
    return m_EglSurface;
}

const EGLContext& CRenderInstanceOGLBase::EglContext() const
{
    return m_EglContext;
}
#endif
