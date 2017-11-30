//
//  CRenderInstanceOGLBase.hpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 11/9/17.
//  Copyright © 2017 Yevgeniy Logachev. All rights reserved.
//

#ifndef CRENDERINSTANCEOGLBASE_H
#define CRENDERINSTANCEOGLBASE_H

#include "IRenderInstance.h"

namespace jam {
    
CLASS_PTR(CRenderInstanceOGLBase)
    
class CRenderInstanceOGLBase : public IRenderInstance
{
    JAM_OBJECT
public:
    CRenderInstanceOGLBase(UIView* glkView,
                           EAGLContext* glContext);

#if defined(EGLAPI)
    CRenderInstanceOGLBase(EGLNativeWindowType eglWindow,
                           EGLDisplay eglDisplay,
                           EGLSurface eglSurface,
                           EGLContext eglContext);
#endif
    virtual ~CRenderInstanceOGLBase();
    
    /*
     * Initialize instance object
     */
    virtual void Initialize() override;
    
    /*
     * Deinitialize instance object
     */
    virtual void Shutdown() override;
    
    /*
     * Check if instance is initialized
     */
    virtual bool IsInitialized() const override;
    
#if defined(__OBJC__)
    /*
     * Apple specific
     * Get OpenGL view
     */
    UIView* View() const;
    
    /*
     * Get OpenGL context
     */
    EAGLContext* Context() const;
#endif

#if defined(EGLAPI)
    /*
     * Common 
     * Get Egl window
     */
    const EGLNativeWindowType& EglWindow() const;
    
    /*
     * Get Egl display
     */
    const EGLDisplay& EglDisplay() const;
    
    /*
     * Get Egl surface
     */
    const EGLSurface& EglSurface() const;
    
    /*
     * Get Egl context
     */
    const EGLContext& EglContext() const;
#endif
    
private:
    bool m_IsInitialized;
    
#if defined(__OBJC__)
    UIView* m_GLKView;
    EAGLContext* m_GLContext;
#endif
    
#if defined(EGLAPI)
    EGLNativeWindowType m_EglWindow;
    EGLDisplay m_EglDisplay;
    EGLSurface m_EglSurface;
    EGLContext m_EglContext;
#endif
};
    
}; // namespace jam

#endif /* CRENDERINSTANCEOGLBASE_H */
